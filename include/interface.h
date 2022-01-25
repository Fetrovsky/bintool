#ifndef INTERFACE__INCLUDED
#define INTERFACE__INCLUDED

struct Interface {
    virtual ~Interface() {}
};

class Base_Class: public Interface {
};

#endif  // INTERFACE__INCLUDED

