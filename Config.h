#ifndef CONFIG_H
#define CONFIG_H

// Compile time configuration options

#define VERSION                         0
#define DEFAULT_ITERATIONS              10000
#define DEFAULT_TRAINING                BACKPROP
#define DEFAULT_ACTIVATION_FUNCTION     TYPE_SIGMOID

// Helper macros
#define QUOTE(x) #x
#define QUOTEMACRO(y) QUOTE(y)

#endif
