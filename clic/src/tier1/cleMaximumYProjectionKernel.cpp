
#include "cleMaximumYProjectionKernel.hpp"

namespace cle
{

MaximumYProjectionKernel::MaximumYProjectionKernel(std::shared_ptr<GPU> t_gpu) : 
    Kernel( t_gpu,
            "maximum_y_projection",
            {"src", "dst"}
    )
{
    this->m_Sources.insert({this->m_KernelName, this->m_OclHeader});
}

void MaximumYProjectionKernel::SetInput(Object& t_x)
{
    this->AddObject(t_x, "src");
}

void MaximumYProjectionKernel::SetOutput(Object& t_x)
{
    this->AddObject(t_x, "dst");
}

void MaximumYProjectionKernel::Execute()
{
    this->BuildProgramKernel();
    this->SetArguments();
    this->EnqueueKernel();
}

} // namespace cle
