#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <cstdint>
#include <random>
#include <ctime>

namespace Random
{
    void init();
    int32_t     intInRange      (int32_t lowBound, int32_t highBound);
    uint64_t    uint64InRange   (uint64_t lowBound, uint64_t highBound);

    template<typename RandomEngine>
    class Generator
    {
        public:
            Generator()
            {
                m_device.seed(std::time(nullptr));
            }

            int32_t intInRange (int32_t lowBound, int32_t highBound)
            {
                std::uniform_int_distribution<int32_t> dist(lowBound, highBound);
                return dist (m_device);
            }

            uint64_t uint64InRange (uint64_t lowBound, uint64_t highBound)
            {
                std::uniform_int_distribution<uint64_t> dist(lowBound, highBound);
                return dist (m_device);
            }

        private:
            RandomEngine m_device;
    };
}

#endif // RANDOM_H_INCLUDED
