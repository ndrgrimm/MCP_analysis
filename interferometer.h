#ifndef INTERFEROMETER_H
#define INTERFEROMETER_H

class Interferometer
{
public:
    Interferometer();
    Interferometer(const Interferometer& other);
    ~Interferometer();
    Interferometer& operator=(const Interferometer& other);
    bool operator==(const Interferometer& other) const;
};

#endif // INTERFEROMETER_H
