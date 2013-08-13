extern void system_call();
extern int strlen(const char *);

extern char strt[];

const char message[] = "couldn't you find a better quote???\n";

extern char blah;


int _start(){
	system_call(4, 1, strt, strlen(strt));
	blah = 7;
	system_call(4, 1, message, strlen(message));
        /* now, exit */
        system_call(1, 0);
}
