
#include <random>
#include "clesperanto.hpp"
#include "utils.hpp"

template<class type>
std::array<size_t,3> generate_data(std::vector<type>& arr_1, 
                                   std::vector<type>& valid, size_t width, size_t height, size_t depth)
{
    arr_1.resize(width*height*depth);
    valid.resize(width*height*depth);
    std::fill(arr_1.begin(), arr_1.end(), static_cast<type>(0));
    std::fill(valid.begin(), valid.end(), static_cast<type>(0));
    int central_idx = (width/2) + (height/2)*width + (depth/2) * height * width;
    arr_1[central_idx] = static_cast<type>(1);
    valid[central_idx] = static_cast<type>(1);
    if(width > 1)
    {
        valid[central_idx+1] = static_cast<type>(1);
        valid[central_idx-1] = static_cast<type>(1);
    }
    if(height > 1)
    {
        valid[central_idx+width] = static_cast<type>(1);
        valid[central_idx-width] = static_cast<type>(1);
    }
    if(depth > 1)
    {
        valid[central_idx+(height * width)] = static_cast<type>(1);
        valid[central_idx-(height * width)] = static_cast<type>(1);
    }
    return std::array<size_t,3> {width, height, depth};
}

template<class type>
std::vector<type> run_kernel_with_buffer(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape, "image");
    auto ocl_output = cle.Create<type>(shape, "image");
    cle.DilateSphere(oclArray_A, ocl_output);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
std::vector<type> run_kernel_with_image(std::vector<type>& arr_1, std::array<size_t,3>& shape)
{
    cle::Clesperanto cle;
    cle.Ressources()->SetWaitForKernelToFinish(true);
    auto oclArray_A = cle.Push<type>(arr_1, shape);
    auto ocl_output = cle.Create<type>(shape);
    cle.DilateSphere(oclArray_A, ocl_output);  
    auto output = cle.Pull<type>(ocl_output);  
    return output; 
}

template<class type>
bool test(size_t width, size_t height, size_t depth)
{
    std::vector<type> arr_1, valid;
    std::array<size_t,3> shape = generate_data<type>(arr_1, valid, width, height, depth);
    auto output_buffer = run_kernel_with_buffer<type>(arr_1, shape);
    if (IsDifferent(output_buffer, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using buffer ... FAILED! " << std::endl;
        return true;
    }
    auto output_image  = run_kernel_with_image<type>(arr_1, shape);
    if (IsDifferent(output_image, valid))
    {
        std::cerr << "kernel ("<<width<<","<<height<<","<<depth<<") using image ... FAILED! " << std::endl;
        return true;
    }
    return false;
}

int main(int argc, char **argv)
{
    if (test<float>(10, 5, 3))
    {
        std::cerr << "DilateSphere kernel 3d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 5, 1))
    {
        std::cerr << "DilateSphere kernel 2d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    if (test<float>(10, 1, 1))
    {        
        std::cerr << "DilateSphere kernel 1d ... FAILED! " << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "DilateSphere kernel test ... PASSED! " << std::endl;
    return EXIT_SUCCESS;
}
