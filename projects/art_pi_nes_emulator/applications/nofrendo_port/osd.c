#include <string.h>
#include <math.h>

#include <rtthread.h>

#include <nofrendo.h>
#include <nofconfig.h>
#include <noftypes.h>
#include <osd.h>
#include <event.h>

#include <video.h>
#include <input.h>
#include <audio.h>

char cfg_filename[] = "cfgfile";
static void (*audio_callback)(void *buffer, int length) = NULL;

static void osd_stopsound(void)
{
   audio_callback = NULL;
   /* TODO */
}

static void osd_freeinput(void)
{
   /* TODO */
}

int osd_main(int argc, char *argv[])
{
   /* config file */
   config.filename = cfg_filename;

   /* all done */
   return main_loop(argv[1], system_autodetect);
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname)
{
   strncpy(fullname, shortname, PATH_MAX);
}

/* This gives filenames for storage of saves */
char *osd_newextension(char *string, char *ext)
{
   return string;
}

/* This gives filenames for storage of PCX snapshots */
int osd_makesnapname(char *filename, int len)
{
   return -1;
}

int osd_installtimer(int frequency, void *func, int funcsize, void *counter, int countersize)
{
   static rt_timer_t timer;
   double ideal = 1000 / frequency;
   int interval = round(ideal);

   UNUSED(counter);
   UNUSED(countersize);
   UNUSED(funcsize);

   rt_kprintf("[osd_installtimer], timer frequency = %d\n", frequency);
   rt_kprintf("[osd_installtimer], interval = %d\n", interval);

   // timer = rt_timer_create("osd_timer",
   //                         NULL,
   //                         NULL,
   //                         interval,
   //                         RT_TIMER_FLAG_SOFT_TIMER | RT_TIMER_FLAG_PERIODIC);
   
   // if (NULL != timer)
   // {
   //    rt_timer_start(timer);
   // }

   return 0;
}

/* Flip between full screen and windowed */
void osd_togglefullscreen()
{
   /* TODO */
}

/* Video */

void osd_getvideoinfo(vidinfo_t *info)
{
   info->default_width = DEFAULT_WIDTH;
   info->default_height = DEFAULT_HEIGHT;

   viddriver_t vd = video_get_driver();
   info->driver = &vd;
}

/* Audio */

void osd_getsoundinfo(sndinfo_t *info)
{
   info->sample_rate = DEFAULT_SAMPLERATE;
   info->bps = DEFAULT_BPS;
}

void osd_setsound(void (*playfunc)(void *buffer, int size))
{
   audio_callback = playfunc;
   /* TODO */
}

/* Input */

void osd_getmouse(int *x, int *y, int *button)
{
}

void osd_getinput(void)
{
   event_t func_event;

   int ev = input_joypad_get_event();
   func_event(ev);
}



int osd_init()
{
   /* Link log function */
   log_chain_logfunc(rt_kprintf);

   return 0;
}

/* this is at the bottom, to eliminate warnings */
void osd_shutdown()
{
	osd_stopsound();
	osd_freeinput();
}