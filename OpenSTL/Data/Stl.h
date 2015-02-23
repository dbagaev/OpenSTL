
namespace OpenSTL { 
namespace Data {

class __declspec(dllexport) Stl
{
public:
    Stl();
    Stl(const Stl & stl);
    ~Stl();

    Stl & operator=(const Stl & other);
};

}  // namespace Data
}  // namespace OpenSTL