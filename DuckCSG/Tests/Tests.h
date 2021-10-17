#pragma once

#include <string>

namespace MapTools
{
    enum class TestResult
    {
        Pass,
        Fail
    };

    class Test
    {
    public:
        virtual TestResult run() = 0;

        std::string failDescription() { return m_failDescription; }
    protected:
        void setFailDescription(std::string str) { m_failDescription = str; }
    protected:
        std::string m_failDescription;
    };

    class Test01ParseOneBrush : public Test
    {
    public:
        virtual TestResult run() override;
    };
}
