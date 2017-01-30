# processing-flow-with-multiple-processes

Give an input like: “13+4*6-20/5*3+7” and the result will be calculated using 3 processes

Workflow

	P0 is the main process which gets the expression,
	then sends the expression to p1

	P1 makes the multiplications and divisions,
	then sends the expression to p2

	P2 makes the additions and subtractions,
	then sends the result to P0

	P0 prints the result and waits for another expression

Communication 

	Anonymous pipes

Compile command

	make

Run command

	./p0
