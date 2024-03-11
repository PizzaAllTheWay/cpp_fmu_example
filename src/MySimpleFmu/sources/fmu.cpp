#include "fmu-uuid.h"
#include <array>
#include <cppfmu_cs.hpp>



class MySimpleFmu : public cppfmu::SlaveInstance
{
public:
    // Constructor must be present
    MySimpleFmu()
        {
            MySimpleFmu::Reset();
        }
    // Reset function is called at the beginning of each simulation
    void Reset() override
    {
        MyInput = 0.0;
        MyParameter = 0;
        MyOutput = {0.0, 0.0};
    }

    // When the simulations tells FMU to set FMUVaraibles to specific values
    // This function loops through all the Real Varaibles in XML file, BUT if it finds differemnt data structure like Integer, then it will call function SetInteger() instead, so we must define this funmction to handle Integer variables later
    // NOTE: We put in MyOutput cases here becauise in case there is some outside STC simulkator stuff that wants to completely overwrite our Outputs they have the ability to do so, not necesarry, but nice to have
    void SetReal(const cppfmu::FMIValueReference valueReferences[], std::size_t number_of_value_references, const cppfmu::FMIReal values[]) override
    {
        for (std::size_t index = 0; index < number_of_value_references; ++index) {
            cppfmu::FMIValueReference valueReference = valueReferences[index];
            cppfmu::FMIReal value = values[index];
            switch(valueReference) {
                // Value reference coresponds to value references specified in XML
                case 0:
                    MyInput = value;
                    break;
                case 1:
                    // Correponds to MyParameter which is an Integer. This should never happen
                    break;
                case 2:
                    MyOutput[0] = value;
                    break;
                case 3:
                    MyOutput[1] = value;
                    break;
                
            }
        }
    }

    void SetInteger(const cppfmu::FMIValueReference valueReferences[], std::size_t number_of_value_references, const cppfmu::FMIInteger values[]) override
    {
        for (std::size_t index = 0; index < number_of_value_references; ++index) {
            cppfmu::FMIValueReference valueReference = valueReferences[index];
            cppfmu::FMIInteger value = values[index];
            switch(valueReference) {
                case 1:
                    MyParameter = value;
                    break;    
            }
        }
    }


    // These are functions for giving Outputs from the FMU
    // GetReal wikll give outputs to Real values, wjhilest getIntegers() wil give output to Integer variables
    // NOTE: We not only set the Outputs here but also Inputs and Parameter, this is not Necesarry, BUT without it, the FMU output wouyld not see what kind of inpout we gave into the FMU, which would be truicy to debug or plot data in case of something (no bueno)
    void GetReal(const cppfmu::FMIValueReference valueReferences[], std::size_t number_of_value_references, cppfmu::FMIReal values[]) const override
    {
        for (std::size_t index = 0; index < number_of_value_references; ++index) {
            cppfmu::FMIValueReference valueReference = valueReferences[index];
            switch(valueReference) {
                // Value reference coresponds to value references specified in XML
                case 0:
                    values[index] = MyInput;
                    break;
                case 2:
                    values[index] = MyOutput[0];
                    break;
                case 3:
                    values[index] = MyOutput[1];
                    break;
                
            }
        }
    }

    void GetInteger(const cppfmu::FMIValueReference valueReferences[], std::size_t number_of_value_references, cppfmu::FMIInteger values[]) const override
    {
        for (std::size_t index = 0; index < number_of_value_references; ++index) {
            cppfmu::FMIValueReference valueReference = valueReferences[index];
            switch(valueReference) {
                case 1:
                    values[index] = MyParameter;
                    break;    
            }
        }
    }

    // Begin simulation here
    bool DoStep(cppfmu::FMIReal startOfTimeStep, cppfmu::FMIReal timeStep, cppfmu::FMIBoolean isThisANewTimeStep,
        cppfmu::FMIReal& endOfTimeStep) override
    {
        MyOutput[0] = MyInput + static_cast<cppfmu::FMIReal>(MyParameter);
        MyOutput[1] += timeStep;
        return true;
    }

private:
    // This defines varaibles with the same data structure as in XML file so that when XML file pases down values with set datastructure we can handle them properly
    cppfmu::FMIReal MyInput;
    
    cppfmu::FMIInteger MyParameter;

    std::array<cppfmu::FMIReal, 2> MyOutput;
    
};


// This MUST be here as it initiates the binding between simple C++ code class and the FMUs world
// This function also allows to pass variables like fmuResourceLocation that is filepath to rescources folder when the fmu is runnjing, this parameter can then be passed to the return cppfmu::AllocateUnique<MySimpleFmu>(memory, fmuResourceLocation);
// The same concept for all other parameters displayed in the functiopn, you dont have to use them, but they are here for conveniece
cppfmu::UniquePtr<cppfmu::SlaveInstance> CppfmuInstantiateSlave(
    cppfmu::FMIString /*instanceName*/, cppfmu::FMIString fmuGUID, cppfmu::FMIString /*fmuResourceLocation*/,
    cppfmu::FMIString /*mimeType*/, cppfmu::FMIReal /*timeout*/, cppfmu::FMIBoolean /*visible*/,
    cppfmu::FMIBoolean /*interactive*/, cppfmu::Memory memory, cppfmu::Logger /*logger*/)
{
    if (std::strcmp(fmuGUID, FMU_UUID) != 0) {
        throw std::runtime_error("FMU GUID mismatch");
    }
    // Class in AllocateUnique is the class that inherits from cppfmu::SlaveInstance and contains the FMU code
    return cppfmu::AllocateUnique<MySimpleFmu>(memory);
}


