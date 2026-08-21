#include "ZDrive.h"
// #if USE_ZMDR

Zdrive Zmotor[USE_ZDRIVE_NUM];
float N2DEG(float pos_deg)
{
    return (pos_deg*360);
}

float DEG2N(float pos_deg)
{
    return (pos_deg/360);
}

// 电机落在哪一路 CAN:SPLIT_COUNT = 0 时全部走第一路,否则前 SPLIT_COUNT 个走第一路
static inline bool is_Zdrive_OnFirstBus(uint32_t motor_index)
{
    return (MOTOR_ZDRIVE_SPLIT_COUNT == 0U) || (motor_index < MOTOR_ZDRIVE_SPLIT_COUNT);
}

static const uint8_t s_empty_data[1] = {0};

bool CanFullFlag = false; // 发送队列满标志,由 Zdrive_Enqueue() 置位
CAN_Send_Queue send_queue;
bool isQueueFull(CAN_Send_Queue *send_queue)
{
    if (send_queue->count == 8)
        return true;
    return false;
}

bool isQueueEmpty(CAN_Send_Queue *send_queue)
{
    if (send_queue->count == 0)
        return true;
    return false;
}

void QueueInit(CAN_Send_Queue *send_queue)
{
    send_queue->head = 0;
    send_queue->tail = 0;
    send_queue->count = 0;
}

void EnQueue(CAN_Send_Queue *send_queue, uint32_t id, uint8_t length, uint8_t *data)
{
    if (isQueueFull(send_queue) == false)
    {
        send_queue->TxBuffer[send_queue->tail].TxHeader.StdId = id;
        send_queue->TxBuffer[send_queue->tail].TxHeader.DLC = length;
        memcpy(send_queue->TxBuffer[send_queue->tail].TxData, data, length);
        send_queue->tail = (send_queue->tail + 1) % 8;
        send_queue->count++;
    }
}

void DeQueue(CAN_Send_Queue *send_queue)
{
    if (isQueueEmpty(send_queue) == false)
    {
        send_queue->head = (send_queue->head + 1) % 8;
        send_queue->count--;
    }
}
void ZdriveInit(void) // 初始化
{
    for (uint32_t i = 0; i < USE_ZDRIVE_NUM; i++)
    {
        Zmotor[i].param.GearRatio = 1.0f;
        Zmotor[i].param.ReductionRatio = 1.0f;
        Zmotor[i].valSetPre.pos_deg = 1e5f;
        Zmotor[i].valSetNow.speed_rpm = 0.0f;
        Zmotor[i].valSetNow.pos_deg = 0.0f;
        Zmotor[i].valSetNow.current_A = 0.0f;
        Zmotor[i].statusFlag.Arriveflag = false;
        Zmotor[i].argum.GapCnt = 0;
        Zmotor[i].valReal.pos_deg = 0.0f;
        Zmotor[i].param.kpPos = 2.3f;
        Zmotor[i].param.kiPos = 0.0f;
        Zmotor[i].param.kpVel = 10.0f;
        Zmotor[i].param.kiVel = 0.0f;
        Zmotor[i].mode = Zdrive_Disable; /* 初始 setmode 为 disable,上电即失能 */
        Zmotor[i].modeRead = Zdrive_Disable;
        Zmotor[i].statusFlag.err = Zdrive_Well; /* 上电无错误 */
        Zmotor[i].Begin = false;                /* 初始化完成后由任务层置 true */
        Zmotor[i].statusFlag.ZeroPoint = 0.0f;
        Zmotor[i].statusFlag.Zeroflag = false;
    }
}

void ZdriveSet(float data, uint8_t id, uint8_t set_code)
{
    if (id == 0U)
    {
        id = 0xFU; /* broadcast address */
        EnQueue(&send_queue, (id | ((uint32_t)set_code << 4U)), 0, (uint8_t *)s_empty_data);
    }
    else
    {
        if (id > USE_ZDRIVE_NUM)
        {
            return;
        }
        if ((set_code == PosIn) || (set_code == Pur))
        {
            data = DEG2N(data) * Zmotor[id - 1U].param.ReductionRatio;
        }
        else if (set_code == VelIn)
        {
            data /= (60.0f / Zmotor[id - 1U].param.ReductionRatio);
        }
        EnQueue(&send_queue, id | ((uint32_t)set_code << 4U), 4U, (uint8_t *)&data);
    }
   
}

// 数据接收模块，数据存在对应的Zmotor[]里
void ZdriveReceive(CAN_RxHeaderTypeDef Rxheader, uint8_t *Rx_Data)
{
    uint32_t control_id = (uint32_t)(Rxheader.StdId & 0xFU);
    uint32_t operation_id = Rxheader.StdId >> 4U;
    float tmp_pos = 0.0f;
    int16_t tmp_vel = 0;
    int16_t tmp_cur = 0;

    /* ZDrive frames are standard IDs below 0x400.  VESC status frames on the
       same bus use standard IDs >= 0x0900 and must not be parsed here. */
    if (Rxheader.IDE != CAN_ID_STD)
    {
        return;
    }
    if ((Rxheader.StdId >> 8U) >= 9U)
    {
        return;
    }

    if ((control_id < 1) || (control_id > USE_ZDRIVE_NUM))
    {
        return;
    }

    uint32_t motor_index = control_id - 1U;

    if (Rxheader.DLC == 4U)
    {
        switch (operation_id)
        {
        case Pur:
            Zmotor[motor_index].valPre.pos_deg = Zmotor[motor_index].valReal.pos_deg;
            memcpy(&Zmotor[motor_index].valReal.pos_deg, Rx_Data, sizeof(float));
            Zmotor[motor_index].valReal.pos_deg = N2DEG(Zmotor[motor_index].valReal.pos_deg) /
                                                  Zmotor[motor_index].param.ReductionRatio;
            break;

        case Cur_M:
            memcpy(&Zmotor[motor_index].valReal.current_A, Rx_Data, sizeof(float));
            Zmotor[motor_index].valReal.torque_Nm = Zmotor[motor_index].valReal.current_A;
            break;

        case Vel:
            memcpy(&Zmotor[motor_index].valReal.speed_rpm, Rx_Data, sizeof(float));
            Zmotor[motor_index].valReal.speed_rpm *= (60.0f / Zmotor[motor_index].param.ReductionRatio);
            break;

        case Mode:
        {
            float temp_mode = 0.0f;
            memcpy(&temp_mode, Rx_Data, sizeof(float));
            Zmotor[motor_index].modeRead = (ZdriveMode)(int32_t)temp_mode;
            break;
        }

        case Err:
        {
            float temp_err = 0.0f;
            memcpy(&temp_err, Rx_Data, sizeof(float));
            Zmotor[motor_index].statusFlag.err = (ZdriveErr)(int32_t)temp_err;
            break;
        }

        case PosIn:
        {
            float temp_pos_in = 0.0f;
            memcpy(&temp_pos_in, Rx_Data, sizeof(float));
            Zmotor[motor_index].valReal.posIn_deg = N2DEG(temp_pos_in) /
                                                    Zmotor[motor_index].param.ReductionRatio;
            break;
        }

        case Vel_Limit:
            memcpy(&Zmotor[motor_index].valReal.velLimit_rpm, Rx_Data, sizeof(float));
            break;

        case Acc_Acu:
            memcpy(&Zmotor[motor_index].valReal.accAcu_rps2, Rx_Data, sizeof(float));
            break;

        case Acc_Dec:
            memcpy(&Zmotor[motor_index].valReal.accDec_rps2, Rx_Data, sizeof(float));
            break;

        default:
            break;
        }
    }
    else if (Rxheader.DLC == 8U)
    {
        memcpy(&tmp_pos, Rx_Data, sizeof(float));

        Zmotor[motor_index].valPre.pos_deg = Zmotor[motor_index].valReal.pos_deg;
        Zmotor[motor_index].valReal.pos_deg = ((tmp_pos) / (float)0xffffffffU *
                                                   (POU - POD) +
                                               POD) /
                                              Zmotor[motor_index].param.ReductionRatio;

        memcpy(&tmp_vel, Rx_Data + 4U, sizeof(int16_t));
        Zmotor[motor_index].valReal.speed_rpm = (float)tmp_vel / (float)0xffffU *
                                                    (2.0f * Velocity_Limit) -
                                                Velocity_Limit;

        memcpy(&tmp_cur, Rx_Data + 6U, sizeof(int16_t));
        Zmotor[motor_index].valReal.current_A = (float)tmp_cur / (float)0xffffU *
                                                    (2.0f * Current_Limit) -
                                                Current_Limit;
    }
}

void ZdriveAsk(uint8_t id, uint8_t ask_code)
{
    if (id == 0U)
    {
        id = 0xFU;
    }

    EnQueue(&send_queue, (id | ((uint32_t)ask_code << 4U)), 0, (uint8_t *)s_empty_data);
    
}

void ZdriveSetPVT(float speed, float angle, uint8_t id)
{
    uint8_t data[8] = {0};

    if ((id == 0U) || (id > USE_ZDRIVE_NUM))
    {
        return;
    }
    uint32_t vel_mod_u32 = (uint32_t)((speed + Velocity_Limit) /
                                      (2.0f * Velocity_Limit) * (float)0xffffffffU);
    uint32_t pos_mod_u32 = (uint32_t)(((angle - POD) / (POU - POD)) *
                                      (float)0xffffffffU);

    memcpy(data, &vel_mod_u32, sizeof(uint32_t));
    memcpy(data + 4U, &pos_mod_u32, sizeof(uint32_t));

    EnQueue(&send_queue, (id | ((uint32_t)PVT_Frame << 4U)), 8U, data);
}

void ZdriveSetPID(float value, uint8_t id, uint8_t pid_code)
{
    uint8_t data[8] = {0};
    uint32_t val_u32;

    if ((id == 0U) || (id > USE_ZDRIVE_NUM))
    {
        return;
    }

    val_u32 = (uint32_t)(value / 100.0f * (float)0xffffffffU);

    memcpy(data, &val_u32, sizeof(uint32_t));

    EnQueue(&send_queue, (id | ((uint32_t)pid_code << 4U)), 8U, data);
}

void ZdriveSetPosVelLimit(float vel_limit, uint8_t id)
{
    uint8_t data[8] = {0};

    if ((id == 0U) || (id > USE_ZDRIVE_NUM))
    {
        return;
    }

    memcpy(data, &vel_limit, sizeof(float));

    EnQueue(&send_queue, (id | ((uint32_t)Vel_Limit << 4U)), 4U, data);
}

void ZdriveSetAccel(float ace, uint8_t id)
{
    uint8_t data[8] = {0};

    if ((id == 0U) || (id > USE_ZDRIVE_NUM))
    {
        return;
    }

    memcpy(data, &ace, sizeof(float));

    EnQueue(&send_queue, (id | ((uint32_t)Acc_Acu << 4U)), 4U, data);
    EnQueue(&send_queue, (id | ((uint32_t)Acc_Dec << 4U)), 4U, data);

    ZdriveAsk(id, Acc_Acu);
    ZdriveAsk(id, Acc_Dec);
}

void ZdriveSetVelLimit(float vel, uint8_t id)
{
    if (id == 0U)
    {
        id = 0xFU; /* broadcast address */
    }
    else
    {
        if (id > USE_ZDRIVE_NUM)
        {
            return;
        }
        /* rpm → rps 换算,和 ZdriveSet 的 VelIn 一致(输出端速度) */
        vel /= (60.0f / Zmotor[id - 1U].param.ReductionRatio);
    }

    EnQueue(&send_queue, (id | ((uint32_t)Vel_Limit << 4U)), 4U, (uint8_t *)&vel);
}

void ZdriveParamConfig(uint8_t id, ZdriveParam param)
{
    ZdriveParam *p;

    if ((id == 0U) || (id > USE_ZDRIVE_NUM))
    {
        return;
    }

    p = &Zmotor[id - 1U].param;

    /* 哪一项 PID 有改动就下发哪一项 */
    if (param.kpPos != p->kpPos)
    {
        ZdriveSetPID(param.kpPos, id, PID_POS_P);
    }
    if (param.kiPos != p->kiPos)
    {
        ZdriveSetPID(param.kiPos, id, PID_POS_I);
    }
    if (param.kpVel != p->kpVel)
    {
        ZdriveSetPID(param.kpVel, id, PID_VEL_P);
    }
    if (param.kiVel != p->kiVel)
    {
        ZdriveSetPID(param.kiVel, id, PID_VEL_I);
    }

    *p = param;
}

/* ---- Switch 状态机:判断条件 modeRead != mode ----
   驱动尚未确认目标模式,持续下发 Mode 命令并查询,直至 read == set 进入运行。
   各模式可在切换期间预处理设定值(如 Speed 清零、Position 对齐当前 posIn),
   确认前不执行周期指令。 */
// 可以放小巧思
static void Zdrive_SwitchMachine(Zdrive *motor, uint8_t id)
{
    
    switch (motor->mode)
    {
    case Zdrive_Speed:
        motor->valSetNow.speed_rpm = 0.f;
        motor->valSetPre.speed_rpm = 0.f;
        break;
    case Zdrive_Postion:
        motor->valSetNow.pos_deg = motor->valReal.pos_deg;
        motor->valSetPre.pos_deg = motor->valReal.pos_deg;
        break;
    case Zdrive_Disable:
    case Zdrive_Current:
    default:
        break;
    }


    // 始终下发模式命令并查询
    ZdriveSet((float)motor->mode, id, Mode);
    ZdriveAsk(id, Mode);
}

/* ---- 运行状态机:判断条件 modeRead == mode,按模式执行周期指令 ---- */
static void Zdrive_RunMachine(Zdrive *motor, uint8_t id)
{
    switch (motor->mode)
    {
    case Zdrive_Speed:
        if (fabs(motor->valSetNow.speed_rpm - motor->valSetPre.speed_rpm) > 0.1f)
        {
            ZdriveSet(motor->valSetNow.speed_rpm, id, VelIn);
            motor->valSetPre.speed_rpm = motor->valSetNow.speed_rpm;
        }
        break;

    case Zdrive_Current:
        // ZDrive MIT 帧格式,未适配
        ZdriveSet(motor->valSetNow.current_A, id, CurIn);
        break;

    case Zdrive_Postion:
        if (!motor->pvtparam.PVTflag)
        {
            if (fabs(motor->valSetNow.pos_deg - motor->valSetPre.pos_deg) >= 0.1)
            {
                motor->valSetPre.pos_deg = motor->valSetNow.pos_deg;
                ZdriveSet(motor->valSetNow.pos_deg, id, PosIn);
            }
            else if (motor->valSetNow.pos_deg == 0.0f &&
                     fabs(motor->valReal.posIn_deg) >= 0.1)
            {
                ZdriveSet(motor->valSetNow.pos_deg, id, PosIn);
            }
        }
        else
        {
            ZdriveSetPVT(motor->valSetNow.speed_rpm, motor->valSetNow.pos_deg, id);
        }
        break;

    case Zdrive_Disable:
    default:
        /* Disable 由驱动自身保持,无周期指令 */
        break;
    }
}

// 错误处理:err != Well 时在此处理,内容暂空白(错误码定义见 ZdriveErr)
static void Zdrive_ErrHandle(Zdrive *motor)
{
    if (motor->statusFlag.err == Zdrive_Well)
    {
        return;
    }

    // TODO: 按错误码处理,如强制 mode = Zdrive_Disable 等
}

void ZdriveFunc(void)
{
    uint32_t i;
    for (i = 0; i < USE_ZDRIVE_NUM; i++)
    {
        // Begin == false:初始化未完成,跳过该电机
        if (!Zmotor[i].Begin)
        {
            continue;
        }

        // 错误处理
        Zdrive_ErrHandle(&Zmotor[i]);

        // Switch 状态机
        if (Zmotor[i].modeRead != Zmotor[i].mode)
        {
            Zdrive_SwitchMachine(&Zmotor[i], (uint8_t)(i + 1U));
            continue;
        }

        // 运行状态机
        Zdrive_RunMachine(&Zmotor[i], (uint8_t)(i + 1U));
    }

    // 任何时候都执行的 ask:广播查询反馈,入队时自动覆盖两条总线
    ZdriveAsk(0, Pur);
    // ZdriveAsk(0, PosIn);
    ZdriveAsk(0, Vel);
    
}
// #endif /* USE_ZMDR */
