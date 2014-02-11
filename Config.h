#ifndef CONFIG_H
#define CONFIG_H

// Compile time configuration options

//#define MULTITHREAD
#define VERSION                         0
#define DEFAULT_ITERATIONS              10000
#define DEFAULT_TRAINING                BACKPROP
#define DEFAULT_ACTIVATION_FUNCTION     TYPE_SIGMOID
#define DEFAULT_LEARNING_RATE           0.1
#define DEFAULT_STATUS_BAR_REFRESH      50

// Helper macros
#define QUOTE(x) #x
#define QUOTEMACRO(y) QUOTE(y)

#endif
