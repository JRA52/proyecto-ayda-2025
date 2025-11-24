#include <iostream>
#include <string>
#include <fstream>  
#include <iomanip>  
#include "ImageGraph.hpp"
#include "Segmenter.hpp"
#include "Classifier.hpp"

extern int g_total_neu;
extern int g_total_mon;
extern int g_total_lin;
extern int g_total_eos;
extern int g_total_atp;

int main(int argc, char* argv[]) 
{
    if (argc < 2) 
    {
        std::cout << "Uso: " << argv[0] << " <imagen1> [imagen2 ...]" << std::endl;
        return 1;
    }

    double umbral_segmentacion = 15.0; 

    for (int i = 1; i < argc; ++i) 
    {
        std::string imagen = argv[i];
        std::cout << "\n========================================" << std::endl;
        std::cout << " PROCESANDO: " << imagen << std::endl;
        std::cout << "========================================" << std::endl;

        ImageGraphBuilder builder;
        if (!builder.load_and_build(imagen)) 
        {
            std::cerr << "ERROR al cargar " << imagen << std::endl;
            continue;
        }
        std::cout << "Dimensiones: " << builder.get_width() << "x" << builder.get_height() << std::endl;

        GraphSegmenter segmenter;
        auto segmentos = segmenter.segmentar(builder.graph, umbral_segmentacion);
        
        CellClassifier classifier;
        classifier.analizar_segmentos(segmentos, builder.get_width(), builder.get_height());
        classifier.contar_celulas();
    }

    int total_valido = g_total_neu + g_total_mon + g_total_lin + g_total_eos;
    
    double pct_neu = (total_valido > 0) ? (g_total_neu * 100.0 / total_valido) : 0.0;
    double pct_mon = (total_valido > 0) ? (g_total_mon * 100.0 / total_valido) : 0.0;
    double pct_lin = (total_valido > 0) ? (g_total_lin * 100.0 / total_valido) : 0.0;
    double pct_eos = (total_valido > 0) ? (g_total_eos * 100.0 / total_valido) : 0.0;

    if (argc > 1) 
    {
        std::cout << "\n\n########################################" << std::endl;
        std::cout << "RESUMEN TOTAL DEL FROTIS (" << (argc-1) << " campos)" << std::endl;
        std::cout << "########################################" << std::endl;
        std::cout << "Neutrófilos Totales: " << g_total_neu << std::endl;
        std::cout << "Monocitos Totales:   " << g_total_mon << std::endl;
        std::cout << "Linfocitos Totales:  " << g_total_lin << std::endl;
        std::cout << "Eosinófilos Totales: " << g_total_eos << std::endl;
        std::cout << "Células Atípicas:    " << g_total_atp << std::endl;
        
        if (total_valido > 0) 
        {
            std::cout << "\n--- PORCENTAJES (Fórmula Leucocitaria) ---" << std::endl;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "NEU: " << pct_neu << "%" << std::endl;
            std::cout << "MON: " << pct_mon << "%" << std::endl;
            std::cout << "LIN: " << pct_lin << "%" << std::endl;
            std::cout << "EOS: " << pct_eos << "%" << std::endl;
        }
    }

    std::ofstream archivo("reporte_medico.csv"); 

    if (archivo.is_open()) 
    {
        archivo << "PARAMETRO,CONTEO TOTAL,PORCENTAJE (%)" << std::endl;
        
        archivo << std::fixed << std::setprecision(2);

        archivo << "Neutrofilos," << g_total_neu << "," << pct_neu << "%" << std::endl;
        archivo << "Monocitos,"   << g_total_mon << "," << pct_mon << "%" << std::endl;
        archivo << "Linfocitos,"  << g_total_lin << "," << pct_lin << "%" << std::endl;
        archivo << "Eosinofilos," << g_total_eos << "," << pct_eos << "%" << std::endl;
        archivo << "Atipicas,"    << g_total_atp << ",-" << std::endl; // Atipicas no llevan % de formula
        
        archivo << ",," << std::endl; 
        archivo << "TOTAL VALIDO," << total_valido << ",100.00%" << std::endl;
        archivo << "Total Campos," << (argc-1) << "," << std::endl;

        archivo.close();
        std::cout << "\n[EXITO] Se ha generado el archivo 'reporte_medico.csv'" << std::endl;
    } else 
    {
        std::cerr << "\n[ERROR] No se pudo crear el archivo CSV." << std::endl;
    }

    return 0;
}