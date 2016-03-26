typedef enum { OFF, ON } led_state;

/*
    Init Functions
*/
void initOnBoardLEDs();
void init1hzTimer3();
void init1000hzTimer3();
void init1000hzTimer0();
void initInputButtons();

/*
    LED Functions
*/
void yellow(led_state);
void red(led_state);
void green(led_state);

void toggleOnBoardGreen();
void toggleOnBoardYellow();
void toggleOnBoardRed();

void gpioRed(led_state);
void gpioGreen(led_state);

/*
    Time Functions
*/
uint32_t systemUptime();
void resetUptime();
uint32_t uptime_ms;
