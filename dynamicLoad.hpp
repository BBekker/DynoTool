
#include <chrono>

class DynamicLoad
{

    private:
    static constexpr double rpm_to_ms = 1 / (60 * 10.8 * (14.5 * (2.56 / 100) * 3.141592));
    static constexpr double filter = 0.1;

    //Internal variables
    double speed = 0.0;
    double acceleration = 0.0;


    std::chrono::steady_clock::time_point last_sample;

    // Settings
    double constant, inertia, v_drag, v2_drag;

    public:

    void newSample(double speed)
    {
        auto now = std::chrono::steady_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::seconds>(now - last_sample);
        double newSpeed = speed * this->filter + this->speed * (1.0 - filter);
        this->acceleration = (newSpeed - this->speed) * dt.count();
        this->speed = newSpeed;
    }

    double getTorque()
    {
        double speedms = this->speed * this->rpm_to_ms;
        double torque = constant + inertia*this->acceleration + speedms * (v_drag + (speedms * v2_drag));
        return torque;
    }


};
