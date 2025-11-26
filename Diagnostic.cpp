#include "Diagnostic.hpp"

std::vector<DiagnosisResult> MedicalDiagnostic::diagnose(double p_neu, double p_lin, double p_mon, double p_eos) 
{
    std::vector<DiagnosisResult> results;

    bool positive = false;

    if (p_neu > 75.0) 
    {        
        results.push_back({"NEUTROPHILIA", "High", "Possible acute bacterial infection or severe inflammation."});
        positive = true;
    } 
    else if (p_neu < 40.0) 
    {
        results.push_back({"NEUTROPENIA", "Medium", "Possible viral infection, drugs, or aplasia."});
        positive = true;
    }

    if (p_lin > 45.0) {
        results.push_back({"LYMPHOCYTOSIS", "High", "Compatible with viral infection or lymphoproliferative process."});
        positive = true;
    } 
    else if (p_lin < 15.0) 
    {
        results.push_back({"LYMPHOPENIA", "Low", "Possible stress, corticosteroids, or immunosuppression."});
        positive = true;
    }

    if (p_eos > 6.0) {
        results.push_back({"EOSINOPHILIA", "High", "Strong indicator of active allergy or parasitosis."});
        positive = true;
    }

    if (p_mon > 12.0) 
    {
        results.push_back({"MONOCYTOSIS", "Medium", "Possible chronic infection or recovery phase."});
        positive = true;
    }

    if (!positive) 
    {
        results.push_back({"NORMAL", "High", "Count within normal statistical ranges."});
    }

    return results;
}