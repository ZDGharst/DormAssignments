#ifndef GLOBALS_H
#define GLOBALS_H

inline constexpr int NUM_STUDENTS = 200;
inline constexpr int NUM_STUDENTS_PER_ROOM = 4;
inline constexpr int NUM_ROOMS = NUM_STUDENTS / NUM_STUDENTS_PER_ROOM;
inline constexpr int SIZE_OF_COMPATIBILITIES = NUM_STUDENTS * NUM_STUDENTS;

inline constexpr int INITIAL_TEMPERATURE = 100;
inline constexpr int CHANGES_BEFORE_REDUCTION = 2000;
inline constexpr int ATTEMPTS_BEFORE_REDUCTION = 20000;
inline constexpr double GEOMETRIC_TEMP_REDUCTION = 0.95;

#endif /* GLOBALS_H */