/* ADC button monitor for Helios+
 *
 * This program is primarily intended as an example application.
 * Reads the current adc buttons status (ADC_SW_STATUS) from iio sysfs and starts adc alarm
 * to detect button state change.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <poll.h>
#include <fcntl.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/iio/events.h>
#include <linux/iio/types.h>

/* macros */
#define pr_info(fmt,arg...)     printf("%ld:info:[%s:%d] "fmt, clock(), __func__, __LINE__, ##arg)
#define pr_debug(fmt,arg...)    printf("%ld:debug:[%s:%d] "fmt, clock(),__func__, __LINE__, ##arg)

// calculate elapsed time in ms
#define ELAPSED_TIME_US(now, start) (((now - start) * 1000 * 1000) / CLOCKS_PER_SEC)

/* constants */
typedef enum {
    SW_RELEASED = 0,
    SW1_PRESSED,
    SW2_PRESSED,
    ALL_SW_PRESSED,
    SW_UNKNOWN,
} ADC_SW_STATUS;

#define MAX_BUFFER_SIZE     1024u

// adc switches are connected to ads7924 channel 0 on Helios+
#define ADC_CHANNEL_NAME    "/sys/bus/iio/devices/iio:device2/in_voltage0_input"
#define ADC_EVENT_ENABLE    "/sys/bus/iio/devices/iio:device2/events/in_voltage0_thresh_either_en"
#define ADC_EVENT_THRESH    "/sys/bus/iio/devices/iio:device2/events/in_voltage0_thresh_either_period"
#define ADC_EVENT_READ      "/dev/iio:device2"

#define ADC_SW_OFF_THRESH   1410    // buttons released voltage, 1.41V
#define ADC_SW2_ON_THRESH   1100    // SW2 pressed voltage, 1.1V
#define ADC_SW1_ON_THRESH   660     // SW1 pressed voltage, 0.66V
#define ADC_SW_ALL_THRESH   100     // SW 1&2 are pressed, 0V
#define ADC_SW_THRESH_DIFF  80      // Threshold diff is 80mV
#define ADC_UPPER_THRESH    2500    // Not used, detect released state by polling mode
#define ADC_LOWER_THRESH    (ADC_SW2_ON_THRESH + ADC_SW_THRESH_DIFF) // trigger voltage of button pressed action

/*-------------------------------------------------------------------------*/
/* global variables */
static char fd_buf[MAX_BUFFER_SIZE];

#ifdef PRINT_IIO_EVENT
static const char * const iio_chan_type_name_spec[] = {
	[IIO_VOLTAGE] = "voltage",
	[IIO_CURRENT] = "current",
	[IIO_POWER] = "power",
	[IIO_ACCEL] = "accel",
	[IIO_ANGL_VEL] = "anglvel",
	[IIO_MAGN] = "magn",
	[IIO_LIGHT] = "illuminance",
	[IIO_INTENSITY] = "intensity",
	[IIO_PROXIMITY] = "proximity",
	[IIO_TEMP] = "temp",
	[IIO_INCLI] = "incli",
	[IIO_ROT] = "rot",
	[IIO_ANGL] = "angl",
	[IIO_TIMESTAMP] = "timestamp",
	[IIO_CAPACITANCE] = "capacitance",
	[IIO_ALTVOLTAGE] = "altvoltage",
	[IIO_CCT] = "cct",
	[IIO_PRESSURE] = "pressure",
	[IIO_HUMIDITYRELATIVE] = "humidityrelative",
	[IIO_ACTIVITY] = "activity",
	[IIO_STEPS] = "steps",
	[IIO_ENERGY] = "energy",
	[IIO_DISTANCE] = "distance",
	[IIO_VELOCITY] = "velocity",
	[IIO_CONCENTRATION] = "concentration",
	[IIO_RESISTANCE] = "resistance",
	[IIO_PH] = "ph",
	[IIO_UVINDEX] = "uvindex",
	[IIO_GRAVITY] = "gravity",
};

static const char * const iio_ev_type_text[] = {
	[IIO_EV_TYPE_THRESH] = "thresh",
	[IIO_EV_TYPE_MAG] = "mag",
	[IIO_EV_TYPE_ROC] = "roc",
	[IIO_EV_TYPE_THRESH_ADAPTIVE] = "thresh_adaptive",
	[IIO_EV_TYPE_MAG_ADAPTIVE] = "mag_adaptive",
	[IIO_EV_TYPE_CHANGE] = "change",
};

static const char * const iio_ev_dir_text[] = {
	[IIO_EV_DIR_EITHER] = "either",
	[IIO_EV_DIR_RISING] = "rising",
	[IIO_EV_DIR_FALLING] = "falling"
};

static const char * const iio_modifier_names[] = {
	[IIO_MOD_X] = "x",
	[IIO_MOD_Y] = "y",
	[IIO_MOD_Z] = "z",
	[IIO_MOD_X_AND_Y] = "x&y",
	[IIO_MOD_X_AND_Z] = "x&z",
	[IIO_MOD_Y_AND_Z] = "y&z",
	[IIO_MOD_X_AND_Y_AND_Z] = "x&y&z",
	[IIO_MOD_X_OR_Y] = "x|y",
	[IIO_MOD_X_OR_Z] = "x|z",
	[IIO_MOD_Y_OR_Z] = "y|z",
	[IIO_MOD_X_OR_Y_OR_Z] = "x|y|z",
	[IIO_MOD_LIGHT_BOTH] = "both",
	[IIO_MOD_LIGHT_IR] = "ir",
	[IIO_MOD_ROOT_SUM_SQUARED_X_Y] = "sqrt(x^2+y^2)",
	[IIO_MOD_SUM_SQUARED_X_Y_Z] = "x^2+y^2+z^2",
	[IIO_MOD_LIGHT_CLEAR] = "clear",
	[IIO_MOD_LIGHT_RED] = "red",
	[IIO_MOD_LIGHT_GREEN] = "green",
	[IIO_MOD_LIGHT_BLUE] = "blue",
	[IIO_MOD_LIGHT_UV] = "uv",
	[IIO_MOD_QUATERNION] = "quaternion",
	[IIO_MOD_TEMP_AMBIENT] = "ambient",
	[IIO_MOD_TEMP_OBJECT] = "object",
	[IIO_MOD_NORTH_MAGN] = "from_north_magnetic",
	[IIO_MOD_NORTH_TRUE] = "from_north_true",
	[IIO_MOD_NORTH_MAGN_TILT_COMP] = "from_north_magnetic_tilt_comp",
	[IIO_MOD_NORTH_TRUE_TILT_COMP] = "from_north_true_tilt_comp",
	[IIO_MOD_RUNNING] = "running",
	[IIO_MOD_JOGGING] = "jogging",
	[IIO_MOD_WALKING] = "walking",
	[IIO_MOD_STILL] = "still",
	[IIO_MOD_ROOT_SUM_SQUARED_X_Y_Z] = "sqrt(x^2+y^2+z^2)",
	[IIO_MOD_I] = "i",
	[IIO_MOD_Q] = "q",
	[IIO_MOD_CO2] = "co2",
	[IIO_MOD_VOC] = "voc",
};

static bool event_is_known(struct iio_event_data *event)
{
	enum iio_chan_type type = IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event->id);
	enum iio_modifier mod = IIO_EVENT_CODE_EXTRACT_MODIFIER(event->id);
	enum iio_event_type ev_type = IIO_EVENT_CODE_EXTRACT_TYPE(event->id);
	enum iio_event_direction dir = IIO_EVENT_CODE_EXTRACT_DIR(event->id);

	switch (type) {
	case IIO_VOLTAGE:
	case IIO_CURRENT:
	case IIO_POWER:
	case IIO_ACCEL:
	case IIO_ANGL_VEL:
	case IIO_MAGN:
	case IIO_LIGHT:
	case IIO_INTENSITY:
	case IIO_PROXIMITY:
	case IIO_TEMP:
	case IIO_INCLI:
	case IIO_ROT:
	case IIO_ANGL:
	case IIO_TIMESTAMP:
	case IIO_CAPACITANCE:
	case IIO_ALTVOLTAGE:
	case IIO_CCT:
	case IIO_PRESSURE:
	case IIO_HUMIDITYRELATIVE:
	case IIO_ACTIVITY:
	case IIO_STEPS:
	case IIO_ENERGY:
	case IIO_DISTANCE:
	case IIO_VELOCITY:
	case IIO_CONCENTRATION:
	case IIO_RESISTANCE:
	case IIO_PH:
	case IIO_UVINDEX:
	case IIO_GRAVITY:
		break;
	default:
		return false;
	}

	switch (mod) {
	case IIO_NO_MOD:
	case IIO_MOD_X:
	case IIO_MOD_Y:
	case IIO_MOD_Z:
	case IIO_MOD_X_AND_Y:
	case IIO_MOD_X_AND_Z:
	case IIO_MOD_Y_AND_Z:
	case IIO_MOD_X_AND_Y_AND_Z:
	case IIO_MOD_X_OR_Y:
	case IIO_MOD_X_OR_Z:
	case IIO_MOD_Y_OR_Z:
	case IIO_MOD_X_OR_Y_OR_Z:
	case IIO_MOD_LIGHT_BOTH:
	case IIO_MOD_LIGHT_IR:
	case IIO_MOD_ROOT_SUM_SQUARED_X_Y:
	case IIO_MOD_SUM_SQUARED_X_Y_Z:
	case IIO_MOD_LIGHT_CLEAR:
	case IIO_MOD_LIGHT_RED:
	case IIO_MOD_LIGHT_GREEN:
	case IIO_MOD_LIGHT_BLUE:
	case IIO_MOD_LIGHT_UV:
	case IIO_MOD_QUATERNION:
	case IIO_MOD_TEMP_AMBIENT:
	case IIO_MOD_TEMP_OBJECT:
	case IIO_MOD_NORTH_MAGN:
	case IIO_MOD_NORTH_TRUE:
	case IIO_MOD_NORTH_MAGN_TILT_COMP:
	case IIO_MOD_NORTH_TRUE_TILT_COMP:
	case IIO_MOD_RUNNING:
	case IIO_MOD_JOGGING:
	case IIO_MOD_WALKING:
	case IIO_MOD_STILL:
	case IIO_MOD_ROOT_SUM_SQUARED_X_Y_Z:
	case IIO_MOD_I:
	case IIO_MOD_Q:
	case IIO_MOD_CO2:
	case IIO_MOD_VOC:
		break;
	default:
		return false;
	}

	switch (ev_type) {
	case IIO_EV_TYPE_THRESH:
	case IIO_EV_TYPE_MAG:
	case IIO_EV_TYPE_ROC:
	case IIO_EV_TYPE_THRESH_ADAPTIVE:
	case IIO_EV_TYPE_MAG_ADAPTIVE:
	case IIO_EV_TYPE_CHANGE:
		break;
	default:
		return false;
	}

	switch (dir) {
	case IIO_EV_DIR_EITHER:
	case IIO_EV_DIR_RISING:
	case IIO_EV_DIR_FALLING:
	case IIO_EV_DIR_NONE:
		break;
	default:
		return false;
	}

	return true;
}

static void print_event(struct iio_event_data *event)
{
	enum iio_chan_type type = IIO_EVENT_CODE_EXTRACT_CHAN_TYPE(event->id);
	enum iio_modifier mod = IIO_EVENT_CODE_EXTRACT_MODIFIER(event->id);
	enum iio_event_type ev_type = IIO_EVENT_CODE_EXTRACT_TYPE(event->id);
	enum iio_event_direction dir = IIO_EVENT_CODE_EXTRACT_DIR(event->id);
	int chan = IIO_EVENT_CODE_EXTRACT_CHAN(event->id);
	int chan2 = IIO_EVENT_CODE_EXTRACT_CHAN2(event->id);
	bool diff = IIO_EVENT_CODE_EXTRACT_DIFF(event->id);

	if (!event_is_known(event)) {
		fprintf(stderr, "Unknown event: time: %lld, id: %llx\n",
			event->timestamp, event->id);

		return;
	}

	printf("Event: time: %lld, type: %s", event->timestamp,
	       iio_chan_type_name_spec[type]);

	if (mod != IIO_NO_MOD)
		printf("(%s)", iio_modifier_names[mod]);

	if (chan >= 0) {
        printf(", alarm ch: %d, enabled ch: %d", chan>>4, chan&0xf);
//       if (diff && chan2 >= 0)
//           printf("-%d", chan2);
	}

	printf(", evtype: %s", iio_ev_type_text[ev_type]);

	if (dir != IIO_EV_DIR_NONE)
		printf(", direction: %s", iio_ev_dir_text[dir]);

	printf("\n");
}
#endif

static int get_voltage(int delay_ms)
{
    int len;
    int fd;
    int value = -1;

    // open adc channel
    fd = open(ADC_CHANNEL_NAME, O_RDONLY);
    if (fd == -1) {
        pr_debug("open %s fail!!!", ADC_CHANNEL_NAME);
    }
    else {
        len = read(fd, fd_buf, MAX_BUFFER_SIZE);
        if (len)
        {
            value = (int)strtol(fd_buf, NULL, 10);
            //pr_info("ch0 voltage= %d mV\n", value);
        }

        close(fd);
        if (delay_ms)
            usleep(delay_ms*1000);
    }

    return value;
}

static int set_event_threshold(int upper, int lower)
{
    int len;
    int fd;

    // threshold values to period format
    len = sprintf(fd_buf, "%04d.00%04d", upper, lower);
    pr_debug("event threshold is [%d, %d]\n", upper, lower);

    // write to adc
    fd = open(ADC_EVENT_THRESH, O_RDWR);
    if (fd == -1) {
        pr_debug("open %s fail!!!", ADC_EVENT_THRESH);
        return -errno;
    }
    else {
        len = write(fd, fd_buf, len);
        if (len <= 0)
        {
            pr_debug("write() returned %d, errno= %d\n", len, errno);
            return -errno;
        }
        close(fd);
    }

    return 0;
}

static int set_event_enable(int flag_enable)
{
    int len;
    int fd;
    char enable_str[8];

    if (flag_enable==true)
        strcpy(enable_str, "1");
    else
        strcpy(enable_str, "0");

    // write to adc
    fd = open(ADC_EVENT_ENABLE, O_RDWR);
    if (fd == -1) {
        pr_debug("open %s fail!!!", ADC_EVENT_ENABLE);
        return -errno;
    }
    else {
        len = write(fd, enable_str, strlen(enable_str));
        if (len <= 0) {
            pr_debug("write() returned %d, errno= %d\n", len, errno);
            return -1;
        }
        close(fd);
    }

    return 0;
}

#define CHECK_SW_DURATION       3000 // 3ms

static int check_sw_status(ADC_SW_STATUS *status)
{
    long start_t;
    int voltage;
    int sw_on = false;

    voltage = get_voltage(0);
    start_t = clock();
    pr_debug("CH0= %d\n", voltage);

    while (true) {
        voltage = get_voltage(1);  // read and wait 1ms
        if (ELAPSED_TIME_US(clock(), start_t) >= CHECK_SW_DURATION)
            break;
    }

    if (voltage > (ADC_SW2_ON_THRESH - ADC_SW_THRESH_DIFF)) {
        sw_on = true;
        *status = SW2_PRESSED;
    } else if (voltage > (ADC_SW1_ON_THRESH - ADC_SW_THRESH_DIFF)) {
        sw_on = true;
        *status = SW1_PRESSED;
    } else if (voltage < ADC_SW_ALL_THRESH) {
        sw_on = true;
        *status = ALL_SW_PRESSED;
    } else {
        sw_on = false;
        *status = SW_UNKNOWN;
        pr_debug("Error!, status unknown, CH0= %d\n", voltage);

        int i;
        for (i=0;i<40;i++)
            pr_debug("%d\n", get_voltage(5));
        printf("\n");
    }

    if (sw_on) {
        pr_debug("sw(%d) is pressed, CH0= %d\n", *status, voltage);
    }

    return sw_on;
}

static void wait_sw_released(ADC_SW_STATUS last_status)
{
    int voltage;

    while (true) {
        voltage = get_voltage(50);  // read and wait 50ms
        if (voltage > ADC_SW_OFF_THRESH)
            break;
    }

    pr_debug("sw(%d) is released, CH0= %d\n", last_status, voltage);
}

/*
 * To simplify the design, the button monitoring proc only detect the button pressed event.
 * Once a button is pressed, it will enter a busy loop to wait the button(s) is released.
 *
 */

static int monitor_proc(void)
{
    struct iio_event_data event;
    int ret;
    int fd, event_fd;
    ADC_SW_STATUS sw_last_status, status;
    int pressed;

    fd = open(ADC_EVENT_READ, O_RDONLY);
    if (fd == -1) {
        pr_debug("Failed to open %s\n", ADC_EVENT_READ);
        return -errno;
    }

    ret = ioctl(fd, IIO_GET_EVENT_FD_IOCTL, &event_fd);
    if (ret == -1 || event_fd == -1) {
        ret = -errno;
        if (ret == -ENODEV)
            pr_debug("This device does not support events\n");
        else
            pr_debug("Failed to retrieve event fd\n");

        close(fd);
        return ret;
    }

    close(fd);
    sw_last_status = status = SW_UNKNOWN;
    while (true) {
        // wait until an iio event is received
        ret = read(event_fd, &event, sizeof(event));
        if (ret == -1) {
            if (errno == EAGAIN) {
                pr_debug("nothing available\n");
                continue;
            } else {
                ret = -errno;
                pr_debug("Failed to read event from device");
                break;
            }
        }

        if (ret != sizeof(event)) {
            pr_debug("Reading event failed!\n");
            ret = -EIO;
            break;
        }

        // check button status
        //print_event(&event);
        pressed = check_sw_status(&status);

        // report button event by jni callback
        // (*pEnv)->CallVoidMethod(pEnv, ...

        // wait until the button is released
        if (pressed)
            wait_sw_released(status);

        // re-enable event interrupt for next event
        ret = set_event_enable(true);
    }

    ret = set_event_enable(false);
    close(event_fd);
    return ret;
}

int main()
{
    int ret;
    int voltage;

    fprintf(stderr, "ADC Button Monitor, build date: 20230517_1305\n");

    // disble event first
    ret = set_event_enable(false);

    voltage = get_voltage(0);
    pr_debug("Current channel 0 voltage= %d\n", voltage);

    if ((ret=set_event_threshold(ADC_UPPER_THRESH, ADC_LOWER_THRESH))==0
        && (ret=set_event_enable(true))==0)
    {
        ret = monitor_proc();
    }

    fprintf(stderr, "ADC Button Monitor is terminated: %d\n", ret);
    return 0;
}

