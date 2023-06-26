# 6.005: Software Construction, Spring 2016

Hacking is often marked by unbridled optimism:

- Bad: writing lots of code before testing any of it, assuming that bugs will be nonexistent or else easy to find and fix.
- Bad: keeping all the details in your head, assuming you’ll remember them forever, instead of writing them down in your code.

But software engineering is not hacking. Engineers are pessimists:

- Good: write a little bit at a time, testing as you go.
- Good: document the assumptions that your code depends on.

The goal of 6.005 is learning how to produce software that is:

- **Safe from bugs**. Correctness (correct behavior right now), and defensiveness (correct behavior in the future).
- **Easy to understand**. Has to communicate to future programmers who need to understand it and make changes in it (fixing bugs or adding new features). That future programmer might be you, months or years from now. You’ll be surprised how much you forget if you don’t write it down, and how much it helps your own future self to have a good design.
- **Ready for change**. Software always changes. Some designs make it easy to make changes; others require throwing away and rewriting a lot of code.

There are other important properties of software (like performance, usability, security), and they may trade off against these three. But these are the Big Three that we care about in 6.005, and that software developers generally put foremost in the practice of building software. It’s worth considering every language feature, every programming practice, every design pattern that we study in this course, and understanding how they relate to the Big Three.

([Reading 1: Static Checking](http://web.mit.edu/6.005/www/sp16/classes/01-static-checking/))

## Immutability

The key design principle here is **immutability**: using immutable objects and immutable references as much as possible. Let’s review how immutability helps with the main goals of this course:

- **Safe from bugs**. Immutable objects aren’t susceptible to bugs caused by aliasing. Immutable references always point to the same object.
- **Easy to understand**. Because an immutable object or reference always means the same thing, it’s simpler for a reader of the code to reason about — they don’t have to trace through all the code to find all the places where the object or reference might be changed, because it can’t be changed.
- **Ready for change**. If an object or reference can’t be changed at runtime, then code that depends on that object or reference won’t have to be revised when the program changes.

([Reading 9: Mutability & Immutability](http://web.mit.edu/6.005/www/sp16/classes/09-immutability/))

## Concurrency

*Concurrency* means multiple computations are happening at the same time. Concurrency is everywhere in modern programming, whether we like it or not:

- Multiple computers in a network
- Multiple applications running on one computer
- Multiple processors in a computer (today, often multiple processor cores on a single chip)

In fact, concurrency is essential in modern programming:

- Web sites must handle multiple simultaneous users.
- Mobile apps need to do some of their processing on servers (“in the cloud”).
- Graphical user interfaces almost always require background work that does not interrupt the user. For example, Eclipse compiles your Java code while you’re still editing it.

Being able to program with concurrency will still be important in the future. Processor clock speeds are no longer increasing. Instead, we’re getting more cores with each new generation of chips. So in the future, in order to get a computation to run faster, we’ll have to split up a computation into concurrent pieces.

- Concurrency: multiple computations running simultaneously
- Shared-memory & message-passing paradigms
- Processes & threads
  - Process is like a virtual computer; thread is like a virtual processor
- Race conditions
  - When correctness of result (post-conditions and invariants) depends on the relative timing of events

These ideas connect to our three key properties of good software mostly in bad ways. Concurrency is necessary but it causes serious problems for correctness. We’ll work on fixing those problems in the next few readings.

- **Safe from bugs**. Concurrency bugs are some of the hardest bugs to find and fix, and require careful design to avoid.
- **Easy to understand**. Predicting how concurrent code might interleave with other concurrent code is very hard for programmers to do. It’s best to design your code in such a way that programmers don’t have to think about interleaving at all.
- **Ready for change**. Not particularly relevant here.

([Reading 19: Concurrency](http://web.mit.edu/6.005/www/sp16/classes/19-concurrency/))

## Thread Safety

Recall race conditions: multiple threads sharing the same mutable variable without coordinating what they’re doing. This is unsafe, because the correctness of the program may depend on accidents of timing of their low-level operations.

There are basically four ways to make variable access safe in shared-memory concurrency:

- **Confinement**. Don’t share the variable between threads. This idea is called confinement.
- **Immutability**. Make the shared data immutable. We’ve talked a lot about immutability already, but there are some additional constraints for concurrent programming.
- **Thread-safe data type**. Encapsulate the shared data in an existing thread-safe data type that does the coordination for you.
- **Synchronization**. Use synchronization to keep the threads from accessing the variable at the same time. Synchronization is what you need to build your own thread-safe data type.

A data type or static method is **thread-safe** if it behaves correctly when used from multiple threads, regardless of how those threads are executed, and without demanding additional coordination from the calling code.

- "behaves correctly" means satisfying its specification and preserving its rep invariant;
- "regardless of how threads are executed" means threads might be on multiple processors or time-sliced on the same processor;
- "without additional coordination" means that the data type can’t put preconditions on its caller related to timing, like "you can’t call `get()` while `set()` is in progress."

This reading talked about three major ways to achieve safety from race conditions on shared mutable data:

- Confinement: not sharing the data.
- Immutability: sharing, but keeping the data immutable.
- Thread-safe data types: storing the shared mutable data in a single thread-safe datatype.

These ideas connect to our three key properties of good software as follows:

- **Safe from bugs**. We’re trying to eliminate a major class of concurrency bugs, race conditions, and eliminate them by design, not just by accident of timing.
- **Easy to understand**. Applying these general, simple design patterns is far more understandable than a complex argument about which thread interleaving are possible and which are not.
- **Ready for change**. We’re writing down these justifications explicitly in a thread safety argument, so that maintenance programmers know what the code depends on for its thread safety.

([Reading 20: Thread Safety](http://web.mit.edu/6.005/www/sp16/classes/20-thread-safety/))

## Locks and Synchronization

Here’s the general principle: **the correctness of a concurrent program should not depend on accidents of timing**.

To achieve that correctness, we enumerated four strategies for making code safe for concurrency:

- Confinement: don’t share data between threads.
- Immutability: make the shared data immutable.
- Use existing thread-safe data types: use a data type that does the coordination for you.
- Synchronization: prevent threads from accessing the shared data at the same time.

In this reading, we’ll finish talking about strategy 4, using synchronization to implement your own data type that is safe for **shared-memory concurrency**.

Since race conditions caused by concurrent manipulation of shared mutable data are disastrous bugs — hard to discover, hard to reproduce, hard to debug — we need a way for concurrent modules that share memory to **synchronize** with each other.

**Locks** are one synchronization technique. A lock is an abstraction that allows at most one thread to own it at a time. Holding a lock is how one thread tells other threads: "I'm changing this thing, don't touch it right now."

And threads can interleave their operations in so many different ways that you will never be able to test even a small fraction of all possible executions.

- Make thread safety arguments about your data types, and document them in the code.
- Acquiring a lock allows a thread to have exclusive access to the data guarded by that lock, forcing other threads to block — as long as those threads are also trying to acquire that same lock.
- The monitor pattern guards the rep of a datatype with a single lock that is acquired by every method.
- Blocking caused by acquiring multiple locks creates the possibility of deadlock.

([Reading 23: Locks and Synchronization](http://web.mit.edu/6.005/www/sp16/classes/23-locks/))
