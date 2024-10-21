#include "your_path_to_directoryOptics.h"

std::ostream& operator<<(std::ostream& os, const Optics& optics)
{
    os << "Optics Details:" << std::endl;
    os << "Type: " << optics.getType() << std::endl;
    os << "Material: " << optics.getMaterial() << std::endl;
    os << "Thickness: " << optics.getThickness() << " mm" << std::endl;
    os << "Diopter: " << optics.getDiopter() << std::endl;

    return os;
}
