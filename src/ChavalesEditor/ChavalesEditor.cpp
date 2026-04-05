#include "ChavalesEditor.h"

// Dear ImGui: standalone example application for SDL3 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <algorithm>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <SDL3/SDL.h>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <filesystem>
namespace fs = std::filesystem;

#include "GameConfigurator.h"
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL3/SDL_opengles2.h>
#else
#include <SDL3/SDL_opengl.h>
#endif

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#include "ChavalesEditor.h"
#endif

static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

// false si no es una configuracion valida
static bool checkConfigInput(const std::string& firstScene, 
    const std::string& iconRoot)
{
    std::filesystem::path dirScn(core::GameConfigurator::_scenesRoot);
    std::filesystem::path fileScn =  dirScn / (firstScene + ".lua");

    std::filesystem::path dirIcon(core::GameConfigurator::_assetsRoot);
    std::filesystem::path fileIcon = dirIcon / (iconRoot + ".png");

    return (fs::exists(fileScn) && fs::exists(fileIcon));
}

static void saveConfigInput(bool disabled, 
    const std::string& str1, 
    const std::string& str4,
    const std::string& str5,
    const std::string& str6,
    ImVec4 clear_color, 
    int width,
    int height)
{
    core::GameConfigurator::_useTOML = disabled ? "SI" : "NO";

    core::GameConfigurator::_firstScene = str1;
    core::GameConfigurator::_gameDLL = str4;

    core::GameConfigurator::_windowName = str5;
    std::replace(core::GameConfigurator::_windowName.begin(), core::GameConfigurator::_windowName.end(), ' ', '_');

    core::GameConfigurator::_iconRoot = str6;

    core::GameConfigurator::_clearColor = { clear_color.x, clear_color.y, clear_color.z, 1.0f };

    core::GameConfigurator::_windowWidth = width;
    core::GameConfigurator::_windowHeight = height;
}

// Main code
bool ChavalesEditor::runEditor()
{
    // Setup SDL
    // [If using SDL_MAIN_USE_CALLBACKS: all code below until the main loop starts would likely be your SDL_AppInit() function]
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        printf("Error: SDL_Init(): %s\n", SDL_GetError());
        return 1;
    }

    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
    SDL_WindowFlags window_flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
    SDL_Window* window = SDL_CreateWindow("ChavalesEditor", (int)(800 * main_scale), (int)(500 * main_scale), window_flags);
    if (window == nullptr)
    {
        printf("Error: SDL_CreateWindow(): %s\n", SDL_GetError());
        return 1;
    }
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr)
    {
        printf("Error: SDL_GL_CreateContext(): %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    //style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load Fonts
    // - If fonts are not explicitly loaded, Dear ImGui will select an embedded font: either AddFontDefaultVector() or AddFontDefaultBitmap().
    //   This selection is based on (style.FontSizeBase * style.FontScaleMain * style.FontScaleDpi) reaching a small threshold.
    // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - If a file cannot be loaded, AddFont functions will return a nullptr. Please handle those errors in your code (e.g. use an assertion, display an error and quit).
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType for higher quality font rendering.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
    //style.FontSizeBase = 20.0f;
    //io.Fonts->AddFontDefaultVector();
    //io.Fonts->AddFontDefaultBitmap();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    ImVec4 clear_color = ImVec4(0.118, 0.118, 0.118, 1.0);

    // Main loop
    bool done = false;
#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!done)
#endif
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // Chavales editor
        {
            static bool disabled = false;
            static bool aviso = false;
            static bool incorrectos = false;

            ImGui::Begin("ChavalesEngine Configuracion");

			ImGui::BeginDisabled(disabled);

            static char str1[128] = "";
            ImGui::InputTextWithHint("Escena inicial", "nombre del .lua", str1, IM_COUNTOF(str1), ImGuiInputTextFlags_CharsNoBlank);

            static char str4[128] = "";
            ImGui::InputTextWithHint("Dll del juego", "nombre del .dll", str4, IM_COUNTOF(str4), ImGuiInputTextFlags_CharsNoBlank);

            static char str5[128] = "";
            ImGui::InputTextWithHint("Nombre de la ventana", "nombre de la ventana", str5, IM_COUNTOF(str5));

            static char str6[128] = "";
            ImGui::InputTextWithHint("Ruta del icono ", "icono", str6, IM_COUNTOF(str6), ImGuiInputTextFlags_CharsNoBlank);
            ImGui::SameLine(); HelpMarker("La ruta debe ser relativa al directorio de recursos. No debe contener espacios.");

            ImGui::ColorEdit3("Color del vacio", (float*)&clear_color, ImGuiColorEditFlags_NoAlpha);

            static int width = 1920;
            static int height = 1080;

            int minW = 128;
            int maxW = 7680;

        	int minH = 128;
            int maxH = 4320;

            if (ImGui::InputInt("Ancho de la ventana", &width, ImGuiInputTextFlags_CharsNoBlank))
            {
                if (width < minW) width = minW;
                if (width > maxW) width = maxW;
            }
            if (ImGui::InputInt("Alto de la ventana", &height, ImGuiInputTextFlags_CharsNoBlank))
            {
                if (height < minH) height = minH;
                if (height > maxH) height = maxH;
            }

            if (ImGui::Button("Guardar configuracion"))
            {
                // si no hay input de nada vacio
                if (!((strcmp(str1, "") == 0) || (strcmp(str4, "") == 0) || (strcmp(str5, "") == 0) || (strcmp(str6, "") == 0)))
                {
                    if (checkConfigInput(str1, str6))
                    {
                        saveConfigInput(disabled, str1, str4, str5, str6, clear_color, width, height);
                        core::GameConfigurator::SaveToFile(CONFIGURATOR_PATH);
                    }
                    else incorrectos = true;
                }
                else aviso = true;
            }
            ImGui::SameLine(); HelpMarker("Asegurese de que la configuracion a guardar es correcta.\n Se sobreescribira la ultima configuracion registrada.");

			ImGui::EndDisabled();

            if (ImGui::Button("Empezar"))
            {
                // si no hay input de nada vacio
                if (!((strcmp(str1, "") == 0) || (strcmp(str4, "") == 0) || (strcmp(str5, "") == 0) || (strcmp(str6, "") == 0)))
                {
                    if (checkConfigInput(str1, str6))
                    {
                        saveConfigInput(disabled, str1, str4, str5, str6, clear_color, width, height);
	                    return false;
	                }
                    else incorrectos = true;
                }
                else aviso = true;
            }

            if (aviso) ImGui::Text("RELLENA TODOS LOS CAMPOS");

            if (incorrectos) ImGui::Text("ALGUNOS CAMPOS SON INCORRECTOS");

            ImGui::Checkbox("Usar configuracion anterior", &disabled);
            if (disabled)
            {
                core::GameConfigurator::LoadFromFile(CONFIGURATOR_PATH);
                strcpy_s(str1, sizeof str1, core::GameConfigurator::_firstScene.c_str());
                strcpy_s(str4, sizeof str4, core::GameConfigurator::_gameDLL.c_str());
                strcpy_s(str5, sizeof str5, core::GameConfigurator::_windowName.c_str());
                strcpy_s(str6, sizeof str6, core::GameConfigurator::_iconRoot.c_str());
                clear_color.x = core::GameConfigurator::_clearColor.getRed();
                clear_color.y = core::GameConfigurator::_clearColor.getGreen();
                clear_color.z = core::GameConfigurator::_clearColor.getBlue();
                width = core::GameConfigurator::_windowWidth;
                height = core::GameConfigurator::_windowHeight;
            }
            else
            {
                //strcpy_s(str1, sizeof str1, "");
                //strcpy_s(str4, sizeof str4, "");
                //strcpy_s(str5, sizeof str5, "");
                //strcpy_s(str6, sizeof str6, "");
                //clear_color = ImVec4(0.118, 0.118, 0.118, 1.0);
                //width = 1920;
                //height = 1080;
            }
            ImGui::SameLine(); HelpMarker("Se usara la ultima configuracion guardada en el archivo de configuracion .toml");

            ImGui::TextLinkOpenURL("Documentacion ChavalesEngine", "https://proyecto3-fdi-ucm.github.io/2526-Grupo03-ChavalesEngine/");

            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DestroyContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return true;
}

int ChavalesEditor::startup()
{
#if _DEBUG
    const wchar_t* target_cmd(L"ExecutableProject_d.exe");
#else
    const wchar_t* target_cmd(L"ExecutableProject_r.exe");
#endif

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    std::wcout << "[CHAVALESEDITOR] Ejecutando " << target_cmd << std::endl;

    memset(&si, 0, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    memset(&pi, 0, sizeof(PROCESS_INFORMATION));

    std::wstring s1(L" ExecutableProject_d.exe ");

    std::wstring toml(std::begin(core::GameConfigurator::_useTOML), std::end(core::GameConfigurator::_useTOML));
    s1.append(toml);
    s1.append(L" ");
    
    if (core::GameConfigurator::_useTOML == "NO") // si no hay que usar el toml se necesitan el resto de argumetos
    {
        // argv[3]->primera escena
        std::wstring fs(std::begin(core::GameConfigurator::_firstScene), std::end(core::GameConfigurator::_firstScene));
        s1.append(fs);
        s1.append(L" ");
        // argv[4] -> .dll
        std::wstring dll(std::begin(core::GameConfigurator::_gameDLL), std::end(core::GameConfigurator::_gameDLL));
        s1.append(dll);
        s1.append(L" ");
        // argv[5]->nombre ventana
        std::wstring name(std::begin(core::GameConfigurator::_windowName), std::end(core::GameConfigurator::_windowName));
        s1.append(name);
        s1.append(L" ");
        // argv[6]->icono
        std::wstring icon(std::begin(core::GameConfigurator::_iconRoot), std::end(core::GameConfigurator::_iconRoot));
        s1.append(icon);
        s1.append(L" ");
        // argv[7]->clear color r
        std::string r = std::to_string(core::GameConfigurator::_clearColor.getRed());
        std::wstring rw(std::begin(r), std::end(r));
        s1.append(rw);
        s1.append(L" ");
        // argv[8]->clear color g
        std::string g = std::to_string(core::GameConfigurator::_clearColor.getGreen());
        std::wstring gw(std::begin(g), std::end(g));
        s1.append(gw);
        s1.append(L" ");
        // argv[9]->clear color b
        std::string b = std::to_string(core::GameConfigurator::_clearColor.getBlue());
        std::wstring bw(std::begin(b), std::end(b));
        s1.append(bw);
        s1.append(L" ");
        // argv[10]->ancho
        std::string w = std::to_string(core::GameConfigurator::_windowWidth);
        std::wstring ww(std::begin(w), std::end(w));
        s1.append(ww);
        s1.append(L" ");
        // argv[11]->alto
        std::string h = std::to_string(core::GameConfigurator::_windowHeight);
        std::wstring hw(std::begin(h), std::end(h));
        s1.append(hw);
    }

    // fin
    s1.append(L"\0");

    BOOL rv = CreateProcess(
        target_cmd,
        const_cast<LPWSTR>(s1.c_str()),
        NULL,
        NULL,
        FALSE,
        CREATE_NEW_CONSOLE,
        NULL,
        NULL,
        &si,
        &pi
    );

    if (rv == FALSE) {
        return 1;
    }
    ::CloseHandle(pi.hThread);

    ::WaitForSingleObject(pi.hProcess, INFINITE);
    ::CloseHandle(pi.hProcess);

    return 0;
}

int main()
{
    if (!ChavalesEditor::runEditor())
    {
        int su = ChavalesEditor::startup();
        return su;
    }

    return 0;
}