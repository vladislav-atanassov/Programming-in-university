#pragma once

#include <iostream>
#include <string>

class Optics
{
private:
    std::string type;
    std::string material;
    float thickness;    // in millimeters
    float diopter;
    float price; 

public:
    Optics() : type(""), material(""), thickness(0.0f), diopter(0.0f), price(0.0) {}

    Optics(const std::string& type, const std::string& material, 
        const float& thickness, const float& diopter, const float& price)
        : type(type), material(material), thickness(thickness), diopter(diopter), price(price) {}

    ~Optics() {}

    friend std::ostream& operator<<(std::ostream& os, const Optics& optics);

    std::string getType() const { return type; }
    std::string getMaterial() const { return material; }
    float getThickness() const { return thickness; }
    float getDiopter() const { return diopter; }
    float getPrice() const { return price; }

    void setType(const std::string& type) { this->type = type; }
    void setMaterial(const std::string& material) { this->material = material; }
    void setThickness(float thickness) { this->thickness = thickness; }
    void setDiopter(float diopter) { this->diopter = diopter; }
    void setPrice(float price) { this->price = price; }
};