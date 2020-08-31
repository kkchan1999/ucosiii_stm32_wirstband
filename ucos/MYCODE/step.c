#include "step.h"

int16_t step_cnt = 0;


//读取xyz数据存入均值滤波器，存满进行计算，滤波后样本存入sample,如何读取存满就不多说了。
void filter_calculate(filter_avg_t *filter, axis_info_t *sample)
{
    uint16_t i;
    int16_t x_sum = 0, y_sum = 0, z_sum = 0;
    for (i = 0; i < FILTER_CNT; i++)
    {
        x_sum += filter->info[i].x;
        y_sum += filter->info[i].y;
        z_sum += filter->info[i].z;
    }
    sample->x = x_sum / FILTER_CNT;
    sample->y = y_sum / FILTER_CNT;
    sample->z = z_sum / FILTER_CNT;
}

//在动态阈值结构体初始化时，一定要将max的值都赋值为最小值，min赋值为最大值，这样才有利于动态更新。
void peak_update(peak_value_t *peak, axis_info_t *cur_sample)
{
    static uint16_t sample_size = 0; //要弄成static
    sample_size++;
    if (sample_size > SAMPLE_SIZE)
    {
        /*采样达到50个，更新一次*/
        sample_size = 1;
        peak->oldmax = peak->newmax;
        peak->oldmin = peak->newmin;
        //初始化
        peak_value_init(peak);
    }
    peak->newmax.x = MAX(peak->newmax.x, cur_sample->x);
    peak->newmax.y = MAX(peak->newmax.y, cur_sample->y);
    peak->newmax.z = MAX(peak->newmax.z, cur_sample->z);

    peak->newmin.x = MIN(peak->newmin.x, cur_sample->x);
    peak->newmin.y = MIN(peak->newmin.y, cur_sample->y);
    peak->newmin.z = MIN(peak->newmin.z, cur_sample->z);
}

u8 slid_update(slid_reg_t *slid, axis_info_t *cur_sample)
{
    u8 res = 0;
    if (ABS((cur_sample->x - slid->new_sample.x)) > DYNAMIC_PRECISION)
    {
        slid->old_sample.x = slid->new_sample.x;
        slid->new_sample.x = cur_sample->x;
        res = 1;
    }
    else
    {
        slid->old_sample.x = slid->new_sample.x;
    }
    if (ABS((cur_sample->y - slid->new_sample.y)) > DYNAMIC_PRECISION)
    {
        slid->old_sample.y = slid->new_sample.y;
        slid->new_sample.y = cur_sample->y;
        res = 1;
    }
    else
    {
        slid->old_sample.y = slid->new_sample.y;
    }

    if (ABS((cur_sample->z - slid->new_sample.z)) > DYNAMIC_PRECISION)
    {
        slid->old_sample.z = slid->new_sample.z;
        slid->new_sample.z = cur_sample->z;
        res = 1;
    }
    else
    {
        slid->old_sample.z = slid->new_sample.z;
    }
    return res;
}

/*判断当前最活跃轴*/
u8 is_most_active(peak_value_t *peak)
{
    u8 res = MOST_ACTIVE_NULL;
    int16_t x_change = ABS((peak->newmax.x - peak->newmin.x));
    int16_t y_change = ABS((peak->newmax.y - peak->newmin.y));
    int16_t z_change = ABS((peak->newmax.z - peak->newmin.z));

    if (x_change > y_change && x_change > z_change && x_change >= ACTIVE_PRECISION)
    {
        res = MOST_ACTIVE_X;
    }
    else if (y_change > x_change && y_change > z_change && y_change >= ACTIVE_PRECISION)
    {
        res = MOST_ACTIVE_Y;
    }
    else if (z_change > x_change && z_change > y_change && z_change >= ACTIVE_PRECISION)
    {
        res = MOST_ACTIVE_Z;
    }
    return res;
}

/*判断是否走步*/
void detect_step(peak_value_t *peak, slid_reg_t *slid, axis_info_t *cur_sample)
{

    u8 res = is_most_active(peak);
    switch (res)
    {
    case MOST_ACTIVE_NULL:
    {
        //fix
        break;
    }
    case MOST_ACTIVE_X:
    {
        int16_t threshold_x = (peak->oldmax.x + peak->oldmin.x) / 2;
        if (slid->old_sample.x > threshold_x && slid->new_sample.x < threshold_x)
        {
            step_cnt++;
        }
        break;
    }
    case MOST_ACTIVE_Y:
    {
        int16_t threshold_y = (peak->oldmax.y + peak->oldmin.y) / 2;
        if (slid->old_sample.y > threshold_y && slid->new_sample.y < threshold_y)
        {
            step_cnt++;
        }
        break;
    }
    case MOST_ACTIVE_Z:
    {
        int16_t threshold_z = (peak->oldmax.z + peak->oldmin.z) / 2;
        if (slid->old_sample.z > threshold_z && slid->new_sample.z < threshold_z)
        {
            step_cnt++;
        }
        break;
    }
    default:
        break;
    }
}

void peak_value_init(peak_value_t *peak)
{
//    axis_info_t temp;
//    temp.x = peak->newmax.x;
//    temp.y = peak->newmax.y;
//    temp.z = peak->newmax.z;
//    peak->newmax.x = peak->newmin.x;
//    peak->newmax.y = peak->newmin.z;
//    peak->newmax.z = peak->newmin.z;
//    peak->newmin.x = temp.x;
//    peak->newmin.y = temp.y;
//    peak->newmin.z = temp.z;
	peak->newmax.x=0;
	peak->newmax.y=0;
	peak->newmax.z=0;

	peak->newmin.x=8192;
	peak->newmin.y=8192;
	peak->newmin.z=8192;
}
