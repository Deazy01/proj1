ECS36C Project 1 - Deque

Student: Joshua Kofoworola
Project Status

The project implements both required deque classes:

CMaxSizeDeque for a fixed maximum capacity.
CVariableSizeDeque for a dynamically growing deque.

Both implementations use a circular array with std::any elements. The variable-size implementation increases its storage capacity when the current storage becomes full.

The project currently passes all 24 GoogleTest tests, including the provided tests and eight additional tests.

Coverage testing completed successfully with:

Line coverage:99.2%Function coverage: 100%

Valgrind testing completed successfully for 1024, 2048, 4096, and 8192 iterations with zero detected memory leaks.

Testing

The following commands were used during development:

make
make coverage
make analysis

make runs the GoogleTest suite.

make coverage runs the tests with coverage instrumentation and generates the HTML coverage report in tests/htmlcov.

make analysis runs the analysis program under Valgrind for the required iteration counts and then runs the analysis normally.

Additional Tests
ZeroCapacityDeque - tests behavior when a fixed-size deque has zero capacity.
EmptyDequeOperations - tests operations on an empty deque and verifies failure behavior.
WrapAroundThenGrowth - tests growth after the circular buffer has wrapped around.
Five additional tests were added to the provided test suite:

MixedFrontBackOperations - tests combinations of front and back insertions and removals.
CircularWrapAround - tests circular buffer behavior after the front and back positions wrap around.
VariableDequeGrowth - tests that the variable-size deque grows when its capacity is reached.
GrowthAfterFrontMovement - tests growth after the front position has moved.
DifferentAnyTypes - tests storing different types in std::any, including int, double, and std::string.

All 24 tests pass for the two deque implementations.

Known Issues

No known functional issues were found during testing.

The benchmark results have some variation between runs because execution time can be affected by the operating system, CPU scheduling, cache behavior, memory allocation, and other system activity.

Code References

The implementation was developed from the project requirements and the provided starter code. No external source code was copied into the deque implementations.

Generative AI Use

I used ChatGPT as a programming assistance tool during this project. I used it mainly to understand implementation approaches, debug issues, understand build and testing commands, and review whether my implementation satisfied the project requirements.

I wrote and tested the final implementation myself and verified the behavior using the provided tests, additional tests, coverage, Valgrind, and the analysis program.

Prompt 1

I asked ChatGPT for help understanding how to implement a fixed-capacity deque using a circular array and how Front, Back, PushFront, PushBack, PopFront, and PopBack should update the front index and size.

Response 1

ChatGPT explained the circular-array approach, including maintaining a front index and the current number of elements. It explained that the back position can be calculated using the front position and size, and that modulo arithmetic can be used to wrap positions around the allocated array.

Changes 1

I used these ideas to implement the deque operations in MaxSizeDeque.cpp. I wrote the implementation using the project's existing class interface and then tested it with the provided and additional GoogleTest tests.

Prompt 2

I asked ChatGPT for help implementing the variable-size deque and understanding how to grow the underlying storage while preserving the logical order of the elements.

Response 2

ChatGPT explained that the variable-size deque can allocate a larger array when the current capacity is reached, copy or move the existing elements into the new array in logical deque order, and reset the front position to the beginning of the new array.

Changes 2

I implemented the Grow() operation in VariableSizeDeque.cpp. The implementation doubles the capacity, moves the existing std::any elements into the new array in logical order, replaces the old storage, and resets the front index.

Prompt 3

I asked ChatGPT for help understanding the Makefile requirements, coverage instrumentation, and Valgrind commands required by the project.

Response 3

ChatGPT explained how to compile coverage-specific object files with --coverage, run the test program, generate an lcov report, and use Valgrind with an error exit code to detect definite and indirect memory leaks.

Changes 3

I updated the Makefile to provide the required build, test, coverage, and analysis targets. I then ran the commands and verified that all tests passed, coverage was generated successfully, and Valgrind reported zero memory leaks.
