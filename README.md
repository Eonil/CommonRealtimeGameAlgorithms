README
======
Hoon H.



A collection of classes which contains algorithms that are useful to real-time game code.




**WARNING** API IS UNSTABLE!!!












Design Policy
-------------

-	Assertions and exceptions are different.

	Assertions are immediate crash for corrupted program state. Assertion means the state is 
	**already corrupted**, so it cannot be recovered. Consequently, the program will quit immediately.

	Exceptions are flow control. It can be catched at runtime, and users may do something for it.
	Anyway, this will ultimately cause a crash if users don't handle it. This library provides
	*strong exception safety** for all classes and methods. For the generic template classes, this
	guarantee is conditional by the parameter type classes. Each requirements are noted in each
	class comments.
