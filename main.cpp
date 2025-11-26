<<<<<<< Updated upstream
<<<<<<< Updated upstream
// main.cpp (Versión con Scroll de Log e UI Invertida)
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

=======
=======
>>>>>>> Stashed changes
// main.cpp (Last version I hope)
#include <SFML/Graphics.hpp>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
#include <filesystem>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <deque> 

namespace fs = std::filesystem;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
// --- Configuración Global ---
const std::string BACKEND_EXEC  = "./backend"; 
const std::string CSV_FILENAME  = "reporte_medico.csv";
=======
// --- Global config ---
const std::string BACKEND_EXEC  = "./backend";
>>>>>>> Stashed changes
=======
// --- Global config ---
const std::string BACKEND_EXEC  = "./backend";
>>>>>>> Stashed changes
const std::vector<std::string> FONT_CANDIDATES = {
    "resources/fonts/GoogleSansFlex.ttf",
    "fonts/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf"
};

<<<<<<< Updated upstream
<<<<<<< Updated upstream
// --- Estado Compartido ---
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
std::atomic<bool> g_analyzing(false);
std::atomic<bool> g_finished(false);
std::atomic<float> g_progress(0.0f);
std::mutex g_logMutex;
std::queue<std::string> g_logQueue;
std::string g_lastOutput;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
// Variables X11 (Se mantienen para compatibilidad, pero no se usan activamente)
Window xdnd_source_window = None;

inline void pushLogLine(const std::string &line) {
    std::lock_guard<std::mutex> lk(g_logMutex);
    // Limpiar si el log es muy largo
    if (g_lastOutput.size() > 20000) { 
        g_lastOutput = "[Log truncado]... \n" + g_lastOutput.substr(g_lastOutput.find('\n', g_lastOutput.size() - 5000) + 1);
    }
    g_logQueue.push(line);
    g_lastOutput += line + "\n";
}

// --- Helpers Sistema (Sin cambios) ---
static std::vector<std::string> openFolderWithZenity() {
    std::vector<std::string> out;
    FILE* pipe = popen("zenity --file-selection --directory --title='Seleccionar carpeta'", "r");
    if (!pipe) return out;
    char buf[4096];
    std::string res;
    while (fgets(buf, sizeof(buf), pipe)) res += buf;
    pclose(pipe);
    if (!res.empty()) {
        while (!res.empty() && (res.back() == '\n' || res.back() == '\r')) res.pop_back();
        if (!res.empty()) out.push_back(res);
    }
    return out;
}

static std::string saveFileWithZenity(const std::string &defaultName) {
    std::string cmd = "zenity --file-selection --save --confirm-overwrite --filename=\"" + defaultName + "\" --title='Guardar CSV'";
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "";
    char buf[4096];
    std::string res;
    while (fgets(buf, sizeof(buf), pipe)) res += buf;
    pclose(pipe);
    if (!res.empty()) {
        while (!res.empty() && (res.back() == '\n' || res.back() == '\r')) res.pop_back();
    }
    return res;
}

static bool isImageExt(const fs::path &p) {
    if (!p.has_extension()) return false;
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    static const std::vector<std::string> allowed = {".png",".jpg",".jpeg",".bmp",".tga",".webp"};
    return std::find(allowed.begin(), allowed.end(), ext) != allowed.end();
}

static bool copy_file_simple(const std::string &src, const std::string &dst) {
    std::ifstream in(src, std::ios::binary);
    std::ofstream out(dst, std::ios::binary);
    if (!in || !out) return false;
    out << in.rdbuf();
    return true;
}

// --- Backend Launcher (Sin cambios) ---
void start_analysis_process(const std::string &backendPath, const std::vector<std::string> &imagePaths) {
    if (g_analyzing) return;
    g_analyzing = true;
    g_finished = false;
    g_progress = 0.0f;
    {
        std::lock_guard<std::mutex> lk(g_logMutex);
        while (!g_logQueue.empty()) g_logQueue.pop();
        g_lastOutput.clear();
=======
=======
>>>>>>> Stashed changes
Window xdnd_source_window = None;

inline void pushLogLine(const std::string &line) 
{
    std::lock_guard<std::mutex> lk(g_logMutex);
    if (g_lastOutput.size() > 20000) 
    {
        g_lastOutput = "[Truncated Log]... \n" + g_lastOutput.substr(g_lastOutput.find('\n', g_lastOutput.size() - 5000) + 1);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    }
    g_logQueue.push(line);
    g_lastOutput += line + "\n";
}

<<<<<<< Updated upstream
<<<<<<< Updated upstream
    pushLogLine("Iniciando analisis con " + std::to_string(imagePaths.size()) + " archivos...");

    std::thread([backendPath, imagePaths]() {
        int pipefd[2];
        if (pipe(pipefd) == -1) { pushLogLine("ERROR: pipe() failed"); g_analyzing=false; return; }

        pid_t pid = fork();
        if (pid == -1) { pushLogLine("ERROR: fork() failed"); close(pipefd[0]); close(pipefd[1]); g_analyzing=false; return; }

        if (pid == 0) { // Hijo (Backend)
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[0]); close(pipefd[1]);
            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(backendPath.c_str()));
            std::vector<std::string> storage = imagePaths; 
            for (const auto &p : storage) argv.push_back(const_cast<char*>(p.c_str()));
            argv.push_back(nullptr);
            execv(backendPath.c_str(), argv.data());
            _exit(127);
        } else { // Padre (GUI)
            close(pipefd[1]);
            const int BUF = 4096;
            char buffer[BUF];
            std::string partial;
            ssize_t n;
            while ((n = read(pipefd[0], buffer, BUF)) > 0) {
                partial.append(buffer, buffer + n);
                size_t pos = 0;
                while (true) {
                    size_t nl = partial.find('\n', pos);
                    if (nl == std::string::npos) break;
                    std::string line = partial.substr(pos, nl - pos);
                    pos = nl + 1;
                    pushLogLine(line);
                    if (line.rfind("PROGRESS", 0) == 0) {
                        int idx = 0, tot = 0;
                        if (sscanf(line.c_str(), "PROGRESS %d %d", &idx, &tot) >= 2 && tot > 0) {
                            g_progress = std::min(1.0f, (float)idx / (float)tot);
                        }
                    } else if (line == "DONE") g_progress = 1.0f;
                }
                if (pos > 0) partial = partial.substr(pos);
            }
            close(pipefd[0]);
            waitpid(pid, nullptr, 0);
            
            pushLogLine("[Analisis finalizado]");
            g_progress = 1.0f;
            g_analyzing = false;
            g_finished = true;
        }
    }).detach();
}

// --- Estructuras ---
struct Thumb {
    sf::Texture texture;
    sf::Sprite sprite;
    std::string path;
};

// --- MAIN ---
int main() {
    XInitThreads();
    // Ajuste del ancho para acomodar dos paneles
    const unsigned W = 1400, H = 720; 
    sf::RenderWindow window(sf::VideoMode(W, H), "Guayaba Analyzer v3.0 (Fixed)");
    window.setFramerateLimit(60);

    // Fuente
    sf::Font font;
    bool fontOk = false;
    for (auto &p : FONT_CANDIDATES) { if (font.loadFromFile(p)) { fontOk = true; break; } }
    if (!fontOk) std::cerr << "WARNING: No font found.\n";

    // --- 1. ZONAS DE LA UI (INVERSIÓN AQUÍ) ---
=======
static std::vector<std::string> openFolderWithZenity() 
{
    std::vector<std::string> out;
    FILE* pipe = popen("zenity --file-selection --directory --title='Image layer. Select folder.'", "r");
    if (!pipe)
    {
        return out;
    }
    char buf[4096];
    std::string res;
    while (fgets(buf, sizeof(buf), pipe)) res += buf;
    pclose(pipe);
    if (!res.empty()) 
    {
        while (!res.empty() && (res.back() == '\n' || res.back() == '\r')) res.pop_back();
        if (!res.empty())
        {
            out.push_back(res);
        }
    }
    return out;
}

=======
static std::vector<std::string> openFolderWithZenity() 
{
    std::vector<std::string> out;
    FILE* pipe = popen("zenity --file-selection --directory --title='Image layer. Select folder.'", "r");
    if (!pipe)
    {
        return out;
    }
    char buf[4096];
    std::string res;
    while (fgets(buf, sizeof(buf), pipe)) res += buf;
    pclose(pipe);
    if (!res.empty()) 
    {
        while (!res.empty() && (res.back() == '\n' || res.back() == '\r')) res.pop_back();
        if (!res.empty())
        {
            out.push_back(res);
        }
    }
    return out;
}

>>>>>>> Stashed changes
static bool isImageExt(const fs::path &p) 
{
    if (!p.has_extension())
    {
        return false;
    }
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

    static const std::vector<std::string> allowed = {".jpeg"};
    return std::find(allowed.begin(), allowed.end(), ext) != allowed.end();
}

void start_analysis_process(const std::string &backendPath, const std::vector<std::string> &imagePaths)
{
    if (g_analyzing)
    {
        return;
    }
    g_analyzing = true;
    g_finished = false;
    g_progress = 0.0f;
    {
        std::lock_guard<std::mutex> lk(g_logMutex);
        while (!g_logQueue.empty())
        {
            g_logQueue.pop();
        }
        g_lastOutput.clear();
    }

    pushLogLine("Starting analysis with " + std::to_string(imagePaths.size()) + " files...");

    std::thread([backendPath, imagePaths]() 
    {
        int pipefd[2];
        if (pipe(pipefd) == -1) 
        { 
            pushLogLine("ERROR: pipe() failed"); g_analyzing=false; return; 
        }

        pid_t pid = fork();
        if (pid == -1) 
        { 
            pushLogLine("ERROR: fork() failed"); close(pipefd[0]); close(pipefd[1]); g_analyzing=false; return; 
        }

        if (pid == 0)
        { 
            dup2(pipefd[1], STDOUT_FILENO);
            dup2(pipefd[1], STDERR_FILENO);
            close(pipefd[0]); close(pipefd[1]);
            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(backendPath.c_str()));
            std::vector<std::string> storage = imagePaths; 
            for (const auto &p : storage) 
            {
                argv.push_back(const_cast<char*>(p.c_str()));
            }
            argv.push_back(nullptr);
            execv(backendPath.c_str(), argv.data());
            _exit(127);
        } 
        else 
        {
            close(pipefd[1]);
            const int BUF = 4096;
            char buffer[BUF];
            std::string partial;
            ssize_t n;
            while ((n = read(pipefd[0], buffer, BUF)) > 0) 
            {
                partial.append(buffer, buffer + n);
                size_t pos = 0;
                while (true) 
                {
                    size_t nl = partial.find('\n', pos);
                    if (nl == std::string::npos)
                    {
                        break;
                    }                    
                    std::string line = partial.substr(pos, nl - pos);
                    pos = nl + 1;
                    pushLogLine(line);
                    if (line.rfind("PROGRESS", 0) == 0) 
                    {
                        int idx = 0, tot = 0;
                        if (sscanf(line.c_str(), "PROGRESS %d %d", &idx, &tot) >= 2 && tot > 0) 
                        {
                            g_progress = std::min(1.0f, (float)idx / (float)tot);
                        }
                    } 
                    else if (line == "DONE")
                    {
                        g_progress = 1.0f;
                    }
                }
                if (pos > 0)
                {
                    partial = partial.substr(pos);
                }
            }
            close(pipefd[0]);
            waitpid(pid, nullptr, 0);           
           
            g_progress = 1.0f;
            g_analyzing = false;
            g_finished = true;
        }
    }).detach();
}
struct Thumb 
{
    sf::Texture texture;
    sf::Sprite sprite;
    std::string path;
};

// --- MAIN ---
int main() 
{
    XInitThreads();    
    const unsigned W = 1200, H = 720; 
    sf::RenderWindow window(sf::VideoMode(W, H), "PQC Analyzer");
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontOk = false;
    for (auto &p : FONT_CANDIDATES) 
    { 
        if (font.loadFromFile(p)) 
        { 
            fontOk = true; break; 
        }
    }
    if (!fontOk) 
    {
        std::cerr << "ERROR: Could not load any font. Exiting.\n";
        return -1;
    }
   
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    const float zoneWidth = 550.f;
    const float zoneHeight = 480.f;
    const float zonePad = 20.f;
    const float startY = 50.f;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
    const float rightX = zonePad * 2 + zoneWidth; // Coordenada X para la zona derecha (590)

    // Zona Izquierda: Log de Análisis (NUEVA POSICIÓN)
    sf::RectangleShape logZone(sf::Vector2f(zoneWidth, zoneHeight));
    logZone.setPosition(zonePad, startY); // Izquierda
    logZone.setFillColor(sf::Color(30,30,30)); 
    logZone.setOutlineThickness(2);
    logZone.setOutlineColor(sf::Color(100,100,100));

    // Zona Derecha: Miniaturas (NUEVA POSICIÓN)
    sf::RectangleShape thumbsZone(sf::Vector2f(zoneWidth, zoneHeight));
    thumbsZone.setPosition(rightX, startY); // Derecha
    thumbsZone.setFillColor(sf::Color(30,30,30)); 
    thumbsZone.setOutlineThickness(2);
    thumbsZone.setOutlineColor(sf::Color(100,100,100));
    
    // Etiqueta de la zona de miniaturas (Se mueve a la derecha con thumbsZone)
    sf::Text thumbsLabel("Archivos a analizar (Selecciona Carpeta)", font, 20);
    thumbsLabel.setFillColor(sf::Color(150,150,150));
    thumbsLabel.setPosition(thumbsZone.getPosition().x + 20, thumbsZone.getPosition().y + 20);
    
    // 2. RENDER TEXTURE (Para la zona de miniaturas - sigue usando el tamaño de thumbsZone)
    sf::RenderTexture scrollTexture;
    if (!scrollTexture.create((unsigned)thumbsZone.getSize().x, (unsigned)thumbsZone.getSize().y)) {
        return -1;
    }
    
    // Botones (Mantenidos alineados bajo la zona izquierda: logZone)
    auto createBtn = [&](float x, float y, float w, float h, sf::Color c, const std::string &txt) {
        sf::RectangleShape r(sf::Vector2f(w,h)); r.setPosition(x,y); r.setFillColor(c);
        sf::Text t(txt, font, 16); t.setFillColor(sf::Color::White); t.setPosition(x+10, y+8);
        return std::make_pair(r, t);
    };

    // Los botones se alinean con la nueva zona izquierda (logZone)
    float btnY = logZone.getPosition().y + logZone.getSize().y + 20;
    float btnX = logZone.getPosition().x;
    // Las posiciones relativas se mantienen, solo el punto de anclaje (btnX) es logZone
    auto uiSelect = createBtn(btnX, btnY, 120, 40, sf::Color(0,100,180), "Carpeta");
    auto uiAnalyze = createBtn(btnX + 130, btnY, 140, 40, sf::Color(40,160,80), "Analizar");
    auto uiExport = createBtn(btnX + 280, btnY, 120, 40, sf::Color(180,120,0), "CSV");
    auto uiNew = createBtn(btnX + 410, btnY, 120, 40, sf::Color(180,50,50), "Limpiar");

    // Miniaturas y Logica
=======
    const float rightX = zonePad * 2 + zoneWidth; 

    sf::RectangleShape logZone(sf::Vector2f(zoneWidth, zoneHeight));
    logZone.setPosition(zonePad, startY); 
    logZone.setFillColor(sf::Color(30,30,30)); 
    logZone.setOutlineThickness(2);
    logZone.setOutlineColor(sf::Color(100,100,100));
   
    sf::RectangleShape thumbsZone(sf::Vector2f(zoneWidth, zoneHeight));
    thumbsZone.setPosition(rightX, startY); 
    thumbsZone.setFillColor(sf::Color(30,30,30)); 
    thumbsZone.setOutlineThickness(2);
    thumbsZone.setOutlineColor(sf::Color(100,100,100));    
    
    sf::Text thumbsLabel("Images to analyze (Select Folder)", font, 16);
    thumbsLabel.setFillColor(sf::Color(150,150,150));
    thumbsLabel.setPosition(thumbsZone.getPosition().x + 20, thumbsZone.getPosition().y + 20);    

=======
    const float rightX = zonePad * 2 + zoneWidth; 

    sf::RectangleShape logZone(sf::Vector2f(zoneWidth, zoneHeight));
    logZone.setPosition(zonePad, startY); 
    logZone.setFillColor(sf::Color(30,30,30)); 
    logZone.setOutlineThickness(2);
    logZone.setOutlineColor(sf::Color(100,100,100));
   
    sf::RectangleShape thumbsZone(sf::Vector2f(zoneWidth, zoneHeight));
    thumbsZone.setPosition(rightX, startY); 
    thumbsZone.setFillColor(sf::Color(30,30,30)); 
    thumbsZone.setOutlineThickness(2);
    thumbsZone.setOutlineColor(sf::Color(100,100,100));    
    
    sf::Text thumbsLabel("Images to analyze (Select Folder)", font, 16);
    thumbsLabel.setFillColor(sf::Color(150,150,150));
    thumbsLabel.setPosition(thumbsZone.getPosition().x + 20, thumbsZone.getPosition().y + 20);    

>>>>>>> Stashed changes
    sf::RenderTexture scrollTexture;
    if (!scrollTexture.create((unsigned)thumbsZone.getSize().x, (unsigned)thumbsZone.getSize().y)) 
    {
        return -1;
    }    
    
    auto createBtn = [&](float x, float y, float w, float h, sf::Color c, const std::string &txt) 
    {
        sf::RectangleShape r(sf::Vector2f(w,h)); r.setPosition(x,y); r.setFillColor(c);
        sf::Text t(txt, font, 16); t.setFillColor(sf::Color::White); t.setPosition(x+10, y+8);        
        return std::make_pair(r, t);
    };

    float btnY = logZone.getPosition().y + logZone.getSize().y + 20;
    float btnX = logZone.getPosition().x;
    auto uiSelect = createBtn(btnX, btnY, 120, 40, sf::Color(0,100,180), "Select Folder");
    auto uiAnalyze = createBtn(btnX + 130, btnY, 140, 40, sf::Color(40,160,80), "Analyze");    
    auto uiNew = createBtn(btnX + 410, btnY, 120, 40, sf::Color(180,50,50), "Clear");

<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
    std::deque<Thumb> thumbs;
    const float thumbW = 100.f, thumbH = 75.f, padding = 10.f; 
    const unsigned cols = 5; 
    float scrollYThumbs = 0.f;
    float contentHeightThumbs = 0.f;
    bool analyzeEnabled = false;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
    // Log (Para el recuadro izquierdo)
    sf::Text logText("", font, 12);
    logText.setFillColor(sf::Color::White); 
    
    // Variables de Scroll para el Log
    float scrollYLog = 0.f;
    float logContentHeight = 0.f;
    const float LOG_LINE_HEIGHT = 16.f;
    const float LOG_PADDING = 10.f;

    // Barra de progreso (Debajo de los botones)
    sf::RectangleShape progBar(sf::Vector2f(0, 10));
    progBar.setPosition(logZone.getPosition().x, btnY + 50); // Alineado con la zona izquierda
    progBar.setFillColor(sf::Color::Green);

    // --- X11 Setup (Sin cambios, solo para la ventana) ---
    Display* disp = XOpenDisplay(nullptr);
    if (!disp) return 1;
    Window xid = window.getSystemHandle();
    (void)xid; // Silenciar warning

    // Helper layout (Sin cambios)
    auto recalculateLayout = [&]() {
        float x0 = padding, y0 = padding;
        for (size_t i = 0; i < thumbs.size(); ++i) {
=======
=======
>>>>>>> Stashed changes
    sf::Text logText("", font, 12);
    logText.setFillColor(sf::Color::White); 
    
    float scrollYLog = 0.f;
    float logContentHeight = 0.f;    
    const float LOG_PADDING = 10.f;

    sf::RectangleShape progBar(sf::Vector2f(0, 10));
    progBar.setPosition(logZone.getPosition().x, btnY + 50); 
    progBar.setFillColor(sf::Color::Green);

    Display* disp = XOpenDisplay(nullptr);
    if (!disp)
    {
        return 1;
    } 
    Window xid = window.getSystemHandle();
    (void)xid;

    auto recalculateLayout = [&]() 
    {
        float x0 = padding, y0 = padding;
        for (size_t i = 0; i < thumbs.size(); ++i) 
        {
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
            unsigned col = i % cols;
            unsigned row = i / cols;
            thumbs[i].sprite.setPosition(x0 + col * (thumbW + padding), y0 + row * (thumbH + padding));
        }
        unsigned rows = (thumbs.size() + cols - 1) / cols;
        contentHeightThumbs = std::max((float)thumbsZone.getSize().y, padding + rows * (thumbH + padding));
        analyzeEnabled = !thumbs.empty();
    };

<<<<<<< Updated upstream
<<<<<<< Updated upstream
    auto addThumb = [&](const std::string &path) {
        Thumb t;
        if (!t.texture.loadFromFile(path)) return false;
        t.path = path;
        thumbs.push_back(std::move(t));
        Thumb &ref = thumbs.back();
        ref.sprite.setTexture(ref.texture);
        sf::Vector2u sz = ref.texture.getSize();
        float s = std::min(thumbW / sz.x, thumbH / sz.y);
        ref.sprite.setScale(s, s);
        return true;
    };

    // --- Bucle (Sin cambios funcionales, solo se usan las variables intercambiadas) ---
    bool running = true;
    while (running) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed) running = false;
            
            // Scroll
            if (ev.type == sf::Event::MouseWheelScrolled) {
                float scrollDelta = ev.mouseWheelScroll.delta * 40.f;
                sf::Vector2f m((float)ev.mouseWheelScroll.x, (float)ev.mouseWheelScroll.y);

                // Scroll en la zona de MINIATURAS (Ahora Derecha)
                if (thumbsZone.getGlobalBounds().contains(m)) {
                    scrollYThumbs -= scrollDelta;
                    scrollYThumbs = std::max(0.f, std::min(scrollYThumbs, contentHeightThumbs - thumbsZone.getSize().y));
                }
                
                // Scroll en la zona de LOG (Ahora Izquierda)
                if (logZone.getGlobalBounds().contains(m)) {
                    scrollYLog -= scrollDelta;
                    scrollYLog = std::max(0.f, std::min(scrollYLog, logContentHeight - logZone.getSize().y + LOG_PADDING));
                }
            }

            // Clics (Sin cambios)
            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f m((float)ev.mouseButton.x, (float)ev.mouseButton.y);
                
                if (uiSelect.first.getGlobalBounds().contains(m)) {
                    auto res = openFolderWithZenity();
                    if (!res.empty()) {
                        thumbs.clear(); 
                        int count = 0;
                        for (auto &e : fs::directory_iterator(res[0])) 
                            if (e.is_regular_file() && isImageExt(e.path())) {
                                if (addThumb(e.path().string())) count++;
                            }
                        recalculateLayout();
                        scrollYThumbs = 0;
                        pushLogLine("Carpeta cargada: " + std::to_string(count) + " imagenes encontradas.");
                        scrollYLog = 0;
                    }
                }
                else if (uiAnalyze.first.getGlobalBounds().contains(m) && analyzeEnabled && !g_analyzing) {
                    std::vector<std::string> p; for(auto &t:thumbs) p.push_back(t.path);
                    start_analysis_process(BACKEND_EXEC, p);
                    scrollYLog = 0;
                }
                else if (uiExport.first.getGlobalBounds().contains(m) && g_finished) {
                    std::string d = saveFileWithZenity(CSV_FILENAME);
                    if (!d.empty()) {
                        if (copy_file_simple(CSV_FILENAME, d)) pushLogLine("CSV guardado en: " + d);
                        else pushLogLine("ERROR: No se pudo guardar el CSV en: " + d);
                        scrollYLog = 0;
                    }
                }
                else if (uiNew.first.getGlobalBounds().contains(m)) {
                    thumbs.clear(); recalculateLayout(); g_finished=false; g_progress=0.f; g_lastOutput="";
                    pushLogLine("Interfaz limpiada.");
                    scrollYLog = 0;
                    scrollYThumbs = 0;
                }
            }
        }

        while (XPending(disp)) {
            XEvent xev; XNextEvent(disp, &xev);
        }

        // --- RENDER ---
        { 
            std::lock_guard<std::mutex> lk(g_logMutex); 
            logText.setString(g_lastOutput); 
        }
        
        progBar.setSize(sf::Vector2f(logZone.getSize().x * g_progress, 10)); // El progreso alineado con la zona izquierda (logZone)

        // 1. Dibujar miniaturas en la TEXTURA (El lienzo de las miniaturas)
        scrollTexture.clear(sf::Color(40,40,40)); 
        for (auto &t : thumbs) {
            sf::Vector2f oldPos = t.sprite.getPosition();
            t.sprite.move(0, -scrollYThumbs); 
            scrollTexture.draw(t.sprite);
            t.sprite.setPosition(oldPos); 
        }
        scrollTexture.display();

        // 2. Dibujar todo en la VENTANA
        window.clear(sf::Color(20,20,20));

        // Pegamos la textura del dropzone (miniaturas - DERECHA) como un sprite
        sf::Sprite thumbsZoneSprite(scrollTexture.getTexture());
        thumbsZoneSprite.setPosition(thumbsZone.getPosition());
        window.draw(thumbsZoneSprite);
        
        // Marco y etiqueta de miniaturas (DERECHA)
        sf::RectangleShape thumbsBorder = thumbsZone;
        thumbsBorder.setFillColor(sf::Color::Transparent); 
        window.draw(thumbsBorder);
        if (thumbs.empty()) window.draw(thumbsLabel);

        // ----------------------------------------------------
        // Zona de Log (IZQUIERDA)
        window.draw(logZone);
        
        if (g_analyzing || g_finished || !g_lastOutput.empty()) {
            // Cálculo de altura total del contenido y ajuste del scroll
            sf::Text measureText(logText.getString(), font, 12);
            logContentHeight = measureText.getLocalBounds().height + LOG_PADDING * 2;
            
            // Ajustamos la posición del texto de log para el scroll
            logText.setPosition(logZone.getPosition().x + LOG_PADDING, logZone.getPosition().y + LOG_PADDING - scrollYLog); 
            
            // Recorte de la vista (View)
=======
    auto addThumb = [&](const std::string &path) 
    {
        Thumb t;
        if (!t.texture.loadFromFile(path))
        {
            return false;
        }

        t.path = path;
        thumbs.push_back(std::move(t));
        Thumb &ref = thumbs.back();
        ref.sprite.setTexture(ref.texture);
        sf::Vector2u sz = ref.texture.getSize();
        float s = std::min(thumbW / sz.x, thumbH / sz.y);
        ref.sprite.setScale(s, s);
        return true;
    };

    bool running = true;
    while (running) 
    {
        sf::Event ev;
        while (window.pollEvent(ev)) 
        {
            if (ev.type == sf::Event::Closed)
            {
                running = false;
            } 
            
            if (ev.type == sf::Event::MouseWheelScrolled) 
            {
                float scrollDelta = ev.mouseWheelScroll.delta * 40.f;
                sf::Vector2f m((float)ev.mouseWheelScroll.x, (float)ev.mouseWheelScroll.y);

=======
    auto addThumb = [&](const std::string &path) 
    {
        Thumb t;
        if (!t.texture.loadFromFile(path))
        {
            return false;
        }

        t.path = path;
        thumbs.push_back(std::move(t));
        Thumb &ref = thumbs.back();
        ref.sprite.setTexture(ref.texture);
        sf::Vector2u sz = ref.texture.getSize();
        float s = std::min(thumbW / sz.x, thumbH / sz.y);
        ref.sprite.setScale(s, s);
        return true;
    };

    bool running = true;
    while (running) 
    {
        sf::Event ev;
        while (window.pollEvent(ev)) 
        {
            if (ev.type == sf::Event::Closed)
            {
                running = false;
            } 
            
            if (ev.type == sf::Event::MouseWheelScrolled) 
            {
                float scrollDelta = ev.mouseWheelScroll.delta * 40.f;
                sf::Vector2f m((float)ev.mouseWheelScroll.x, (float)ev.mouseWheelScroll.y);

>>>>>>> Stashed changes
                if (thumbsZone.getGlobalBounds().contains(m)) 
                {
                    scrollYThumbs -= scrollDelta;
                    scrollYThumbs = std::max(0.f, std::min(scrollYThumbs, contentHeightThumbs - thumbsZone.getSize().y));
                }

                if (logZone.getGlobalBounds().contains(m)) 
                {
                    scrollYLog -= scrollDelta;
                    scrollYLog = std::max(0.f, std::min(scrollYLog, logContentHeight - logZone.getSize().y + LOG_PADDING));
                }
            }

            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2f m((float)ev.mouseButton.x, (float)ev.mouseButton.y);
                
                if (uiSelect.first.getGlobalBounds().contains(m)) 
                {
                    auto res = openFolderWithZenity();
                    if (!res.empty()) 
                    {                        
                        thumbs.clear(); 
                        int count = 0;
                        for (auto &e : fs::directory_iterator(res[0])) 
                            if (e.is_regular_file() && isImageExt(e.path())) 
                            {
                                if (addThumb(e.path().string()))
                                {
                                    count++;
                                }
                            }
                        recalculateLayout();
                        scrollYThumbs = 0;
                        pushLogLine("Folder loaded: " + std::to_string(count) + " images found. ");
                        scrollYLog = 0;
                    }
                }
                else if (uiAnalyze.first.getGlobalBounds().contains(m) && analyzeEnabled && !g_analyzing) 
                {
                    std::vector<std::string> p; for(auto &t:thumbs) p.push_back(t.path);
                    start_analysis_process(BACKEND_EXEC, p);
                    scrollYLog = 0;
                }
                
                else if (uiNew.first.getGlobalBounds().contains(m)) 
                {
                    thumbs.clear(); recalculateLayout(); g_finished=false; g_progress=0.f; g_lastOutput="";
                    pushLogLine("Cleaned interface.");
                    scrollYLog = 0;
                    scrollYThumbs = 0;
                }
            }
        }

        while (XPending(disp)) 
        {
            XEvent xev; XNextEvent(disp, &xev);
        }

        { 
            std::lock_guard<std::mutex> lk(g_logMutex); 
            logText.setString(g_lastOutput); 
        }
        
        progBar.setSize(sf::Vector2f(logZone.getSize().x * g_progress, 10));

        scrollTexture.clear(sf::Color(40,40,40)); 
        for (auto &t : thumbs) 
        {
            sf::Vector2f oldPos = t.sprite.getPosition();
            t.sprite.move(0, -scrollYThumbs); 
            scrollTexture.draw(t.sprite);
            t.sprite.setPosition(oldPos); 
        }
        scrollTexture.display();

        window.clear(sf::Color(20,20,20));

        sf::Sprite thumbsZoneSprite(scrollTexture.getTexture());
        thumbsZoneSprite.setPosition(thumbsZone.getPosition());
        window.draw(thumbsZoneSprite);
        
        sf::RectangleShape thumbsBorder = thumbsZone;
        thumbsBorder.setFillColor(sf::Color::Transparent); 
        window.draw(thumbsBorder);
        if (thumbs.empty())
        {
            window.draw(thumbsLabel);
        } 

        window.draw(logZone);
        
        if (g_analyzing || g_finished || !g_lastOutput.empty()) 
        {
            sf::Text measureText(logText.getString(), font, 12);
            logContentHeight = measureText.getLocalBounds().height + LOG_PADDING * 2;            
           
            logText.setPosition(logZone.getPosition().x + LOG_PADDING, logZone.getPosition().y + LOG_PADDING - scrollYLog); 
            
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
            sf::View logView(logZone.getGlobalBounds());
            logView.setViewport(sf::FloatRect(
                logZone.getPosition().x / W, 
                logZone.getPosition().y / H, 
                logZone.getSize().x / W, 
                logZone.getSize().y / H
            ));
            
            window.setView(logView);
            window.draw(logText); 
<<<<<<< Updated upstream
<<<<<<< Updated upstream
            window.setView(window.getDefaultView()); // Restaurar la vista por defecto
            
        } else {
            // Mensaje inicial en la zona de log
            sf::Text initialLogMsg("El log de analisis aparecerá aqui", font, 16);
=======
=======
>>>>>>> Stashed changes
            window.setView(window.getDefaultView());            
        } 
        
        else
        {
            sf::Text initialLogMsg("Analysis log ", font, 16);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
            initialLogMsg.setFillColor(sf::Color(150,150,150));
            initialLogMsg.setPosition(logZone.getPosition().x + 20, logZone.getPosition().y + 20);
            window.draw(initialLogMsg);
        }
<<<<<<< Updated upstream
<<<<<<< Updated upstream
        // ----------------------------------------------------


        // UI de botones y barra de progreso (Alineados con la zona IZQUIERDA)
        window.draw(uiSelect.first); window.draw(uiSelect.second);
        window.draw(uiAnalyze.first); window.draw(uiAnalyze.second);
        
        if (g_finished) {
            window.draw(uiExport.first); window.draw(uiExport.second);
            window.draw(uiNew.first); window.draw(uiNew.second);
        }
        
        window.draw(progBar);

=======
=======
>>>>>>> Stashed changes
        
        window.draw(uiSelect.first); window.draw(uiSelect.second);
        window.draw(uiAnalyze.first); window.draw(uiAnalyze.second);
        
        if (g_finished) 
        {            
            window.draw(uiNew.first);
            window.draw(uiNew.second);
        }       

        window.draw(progBar);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        window.display();
    }
    XCloseDisplay(disp);
    return 0;
}