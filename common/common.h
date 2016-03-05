typedef enum { OFF, ON } led_state;

/*
    Init Functions
*/
void initLEDs();
void init1hzTimer();
void init1000hzTimer();
void initInputButtons();

/*
    LED Functions
*/
void yellow(led_state);
void red(led_state);
void green(led_state);

void toggleGreen();
void toggleYellow();
void toggleRed();

void gpioRed(led_state);
void gpioGreen(led_state);

/*
    Time Functions
*/
int systemUptime();
void resetUptime();
