This is the example from the OpenMaple/Java/Examples page.  It is a very simple
Java Open Maple program that executes a single expression.

This example is primarily intended to be used for learning how to 
build and execute a Java Open Maple application.

In the following examples, $JDKBINDIR refers to the directory in
which your Java development tools are installed. 

Begin by setting the MAPLE environment variable.  The value of $MAPLE should
be the base directory of your Maple installation.  To determine the correct
value for MAPLE, run

kernelopts( mapledir )

Depending on which shell you are using, in particular sh compatible shells
(like bash and Korn shell) will require that the variable be exported before
java is able to access its value.  

For example in an sh compatible shell the following should be used

export MAPLE="/usr/local/maple12"

A csh compatible shell would use the following syntax.

setenv MAPLE "/usr/local/maple12"

Whenever you set an environment variable, you will need to use the same
procedure.

Building test.java is easy, on Windows: 

        $JDKBINDIR/javac -classpath "$MAPLE/java/externalcall.jar;$MAPLE/java/jopenmaple.jar" test.java

and on UNIX: 

        $JDKBINDIR/javac -classpath "$MAPLE/java/externalcall.jar:$MAPLE/java/jopenmaple.jar" test.java

If successful, there should now be a file called test.class in the current
directory. Before this file can be executed, the Java Virtual Machine must be
told where to look for the Java Open Maple native library. This library
connects Java to Maple. It is located in the binary directory of your Maple
installation.  To determine the correct value, run

kernelopts( bindir )

in your version of Maple.  To tell the Java Virtual Machine to look in this
directory, on Windows place this directory within the PATH variable, on UNIX
this directory must be in the dynamic linker's search path variable.  On
Linux and Sun, use LD_LIBRARY_PATH.  On Mac OS X, use DYLD_LIBRARY_PATH.  To
set these variables, use the procedure described above for setting MAPLE.

This class file can be executed using the Java Virtual Machine, by running the
following command, on Windows 

        $JDKBINDIR/java -classpath "$MAPLE/java/externalcall.jar;$MAPLE/java/jopenmaple.jar;." test

and on UNIX: 

        $JDKBINDIR/java -classpath "$MAPLE/java/externalcall.jar:$MAPLE/java/jopenmaple.jar:." test

If all went well, the following output should appear on screen. 

1/2*x^2
Done

