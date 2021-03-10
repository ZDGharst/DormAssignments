#ifndef GLOBALS_H
#define GLOBALS_H

inline constexpr int NUM_STUDENTS = 200;
inline constexpr int NUM_STUDENTS_PER_ROOM = 4;

inline constexpr int NUM_ROOMS = NUM_STUDENTS / NUM_STUDENTS_PER_ROOM;
inline constexpr int SIZE_OF_COMPATIBILITIES = NUM_STUDENTS * NUM_STUDENTS;

#endif /* GLOBALS_H */