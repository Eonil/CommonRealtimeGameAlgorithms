README
======
Hoon H.



A collection of classes which contains algorithms that are useful to real-time game code.




**WARNING** API IS UNSTABLE!!!












Design Policy
-------------

-	This library provides *strong exception safety** for all specific classes and methods. For the generic template classes, this
	guarantee is provided conditionally by the parameter type classes. Each requirements are noted in each class comments.

-	Assertions are all conditionally removeable. If you're sure that your program is fully bug-free (...), then removing of exception
	will make no difference on program execution.











Conceptual Talk
---------------

-	Assertions and exceptions are different.
	Don't be fooled by wording similarity.

	Major difference is **oppotunity of choice**. Exceptions provide you an oppotunity of recovery, but assertions are not. This 
	difference makes exceptions *testable*, and this is very huge.

	Assertions are debugging aid. It means *no will to recover*. Assertions usually checks **already corrupted state**, so they
	cannot be recovered. Consequently, they always crash program immediately on corrupted execution state to prevent further damage.
	If program is fully bug-free, removing of assertions should make no difference on execution and result.

	Exceptions are regular flow control. It means *willing to recover*. Main purpose of exception is providing **choice of recovery**
	to users. (assertions has no choice due to forced crash) This is very useful on test code.
	It requires program state is clean and not corrupted. If some operation is going to corrupt state, user check check it, and except 
	the situation by throwing an exception.
	Because it's a part of regular program, removing exception will break program logics.

-	An uncaught exception means is a bug of omitting regular handling structure.

-	Exceptions are very expensive in most modern C++ compilers. Because it's literally *exceptional* flow, so it's better to make
	other parts faster.
	
-	Generalization of exception is CPS.
