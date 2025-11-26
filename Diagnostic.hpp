#pragma once
#include <string>
#include <vector>
#include <iostream>

struct DiagnosisResult 
{
    std::string condition;
    std::string probability;
    std::string description;
};

class MedicalDiagnostic 
{
public:
    
    static std::vector<DiagnosisResult> diagnose(double p_neu, double p_lin, double p_mon, double p_eos);
};