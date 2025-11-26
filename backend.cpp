#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include "ImageGraph.hpp"
#include "Segmenter.hpp"
#include "Classifier.hpp"
#include "Diagnostic.hpp"

extern int g_total_neu;
extern int g_total_mon;
extern int g_total_lin;
extern int g_total_eos;
extern int g_total_atp;

int main(int argc, char* argv[]) 
{
    int total_images = argc - 1;
    
    if (argc < 2) 
    {
        std::cout << "Uso: " << argv[0] << " <image1> [image2 ...]" << std::endl;
        return 1;
    }

    double segmentation_threshold = 15.0; 

    for (int i = 1; i < argc; ++i) 
    {
        std::string image = argv[i];
        std::cout << "\n========================================\n" << std::endl;
        std::cout << " PROCESSING: " << image << std::endl;

        
        ImageGraphBuilder builder;
        if (!builder.load_and_build(image)) 
        {
            std::cerr << "ERROR loading image " << image << std::endl;
            continue;
        }

        GraphSegmenter segmenter;
        auto segments = segmenter.segment(builder.graph, segmentation_threshold);
        
        CellClassifier classifier;
        classifier.analyze_segments(segments, builder.get_width(), builder.get_height());
        classifier.count_cells();
        std::cout << "PROGRESS " << i << "/" << total_images << std::endl;
    }

    int totally_valid = g_total_neu + g_total_mon + g_total_lin + g_total_eos;
    double pct_neu = 0, pct_mon = 0, pct_lin = 0, pct_eos = 0;

    if (totally_valid > 0) 
    {
        pct_neu = g_total_neu * 100.0 / totally_valid;
        pct_mon = g_total_mon * 100.0 / totally_valid;
        pct_lin = g_total_lin * 100.0 / totally_valid;
        pct_eos = g_total_eos * 100.0 / totally_valid;
    }

    std::vector<DiagnosisResult> diagnoses;
    if (totally_valid > 0) 
    {
        diagnoses = MedicalDiagnostic::diagnose(pct_neu, pct_lin, pct_mon, pct_eos);
    }

    if (argc > 1) 
    {
        std::cout << "\n\n#####################################" << std::endl;
        std::cout << "       AUTOMATED CLINICAL SUMMARY       " << std::endl;
        std::cout << "#####################################\n" << std::endl;
        std::cout << "Neutrophils: " << g_total_neu << " (" << std::fixed << std::setprecision(1) << pct_neu << "%)" << std::endl;
        std::cout << "Lymphocytes:  " << g_total_lin << " (" << pct_lin << "%)" << std::endl;
        std::cout << "Monocytes:   " << g_total_mon << " (" << pct_mon << "%)" << std::endl;
        std::cout << "Eosinophils: " << g_total_eos << " (" << pct_eos << "%)" << std::endl;
        
        
        std::cout << "\n--- SUGGESTED DIAGNOSIS ---" << std::endl;
        for(auto& d : diagnoses) 
        {
            std::cout << ">> " << d.condition << " [" << d.probability << "]" << std::endl;
            std::cout << "   " << d.description << std::endl;
        }
    }
    return 0;
}