/**

\page Reference

\section Generated C++ and QML types

This section describes the types which are generated by the code generator for every interface defined in a QFace file.
We will take an example of interface called "MyInterface", which could look like the following:
\include "mypackage.qface"

\subsection i Generated C++ types

The following table describes the C++ classes which are generated from the QFace definition of "MyInterface".

C++ class name                      | Description
----------------------------------- | -------------
MyInterface                         | This C++ abstract class (or interface) is the direct translation of the QFace interface definition into C++. Any concrete implementation of "MyInterface" should extend somehow this class (not necessarily directly)
MyInterfaceInterfacePropertyAdapter | This C++ abstract class extends "MyInterface" and provides a more convenient base class to write an implementation of "MyInterface".
MyInterfaceQMLFrontend              | This class wraps an instance of "MyInterface" into an object which can be exposed directly to QML. Note that the "MyInterface" class is intentionally not QML-friendly.
MyInterfaceQMLImplementation        | This class is used to enable the implementation of "MyInterface" using the QML language.
MyInterfaceIPCAdapter               | This class is used to make an implementation of "MyInterface" accessible through an IPC.
MyInterfaceIPCProxy                 | This class is an IPC proxy for the "MyInterface" type. A proxy object implements the same interface as the actual implementation object, which typically lives in another process.
MyInterfaceMonitor                  | This class provides a minimalistic Qt-Widget based GUI window which shows the state (property values) of an implementation of "MyInterface". It can also be used to invoke methods (TBD) and to log any signal triggered by the implementation.
MyInterfaceDummy                    | This type is an implementation of "MyInterface" which shows a control GUI where property values can be set manually, and signals can be triggered. It can be useful during development, in order to test the behavior of the UI code under specific conditions, or when an actual implement of the interface is not available yet.



\startuml

abstract class MyInterface #88CCDD {
    C++ abstract class defining the interface
}


class MyInterfaceQMLFrontend #88CCDD {
    This class wraps an instance of "MyInterface" into an object which can be exposed directly to QML
}
MyInterfaceQMLFrontend --> "provider" MyInterface


class MyInterfacePropertyAdapter {
}
MyInterfacePropertyAdapter --|> MyInterface : extends


class MyInterfaceIPCProxy {
    This class is an IPC proxy for the "MyInterface" type
}
MyInterfaceIPCProxy --|> MyInterfacePropertyAdapter : extends


class MyInterfaceDummy {

}
MyInterfaceDummy --|> MyInterfacePropertyAdapter : extends


class MyInterfaceQMLImplementation {
    Used as base component when implementing "Interface" using QML code
}
MyInterfaceQMLImplementation --> "provider" MyInterfacePropertyAdapter

\enduml


\subsection ii Generated QML types

The following table describes the QML types.

QML component name                  | Corresponding C++ class      | Description
----------------------------------- | ---------------------------- | -----------------------
MyInterfaceQMLImplementation        | MyInterfaceQMLImplementation | This class is used to enable the implementation of "MyInterface" using the QML language.
MyInterfaceIPCProxy                 | MyInterfaceQMLFrontend       | This class is an IPC proxy for the "MyInterface" type. A proxy object implements the same interface as the actual implementation object, which typically lives in another process.


\section Relations Relations between QML types

Note how the MyInterfaceQMLFrontend and the MyInterfaceQMLImplementation differ: the latter is used when writing an implementation of an
interfaceu using the QML language, which means all the properties defined in the IDL are writable, and the signals can be triggered, whereas the properties exposed
by MyInterfaceQMLFrontend are typically read-only (if they are defined readonly in the IDL definition).



*/
