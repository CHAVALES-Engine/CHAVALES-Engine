#include "ChavalesEditor.h"

// Dear ImGui: standalone example application for SDL3 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include <iostream>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"
#include <string>
#include <SDL3/SDL.h>

#include <windows.h>
#include <stdio.h>
#include <tchar.h>

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
    SDL_Window* window = SDL_CreateWindow("ChavalesEditor", (int)(1280 * main_scale), (int)(800 * main_scale), window_flags);
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
    //ImGui::StyleColorsLight();

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
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    //IM_ASSERT(font != nullptr);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!done)
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        // [If using SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your SDL_AppEvent() function]
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT)
                done = true;
            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppIterate() function]
        if (SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED)
        {
            SDL_Delay(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // Chavales editor
        {
            static bool disabled = false;

            ImGui::Begin("ChavalesEngine Project Settings");

			ImGui::BeginDisabled(disabled);

            static char str1[128] = "";
            ImGui::InputTextWithHint("Escena inicial", "nombre del .lua", str1, IM_COUNTOF(str1), ImGuiInputTextFlags_CharsNoBlank);

            static char str2[128] = "";
            ImGui::InputTextWithHint("Ruta de las escenas", "ruta escenas", str2, IM_COUNTOF(str2), ImGuiInputTextFlags_CharsNoBlank);

            static char str3[128] = "";
            ImGui::InputTextWithHint("Ruta de los recursos", "ruta recursos", str3, IM_COUNTOF(str3), ImGuiInputTextFlags_CharsNoBlank);

            static char str4[128] = "";
            ImGui::InputTextWithHint("Dll del juego", "nombre del .dll", str4, IM_COUNTOF(str4), ImGuiInputTextFlags_CharsNoBlank);

            static char str5[128] = "";
            ImGui::InputTextWithHint("Nombre de la ventana", "nombre de la ventana", str5, IM_COUNTOF(str5));

            static char str6[128] = "";
            ImGui::InputTextWithHint("Ruta del icono ", "icono", str6, IM_COUNTOF(str6));
            ImGui::SameLine(); HelpMarker("La ruta debe ser relativa al directorio de recursos.");

            ImGui::Checkbox("Demo Window", &show_demo_window);

            ImGui::ColorEdit3("Color del vacio", (float*)&clear_color); 

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
                std::cout << "Escena inicial: " << str1 << std::endl;
                std::cout << "Ruta de las escenas: " << str2 << std::endl;
                std::cout << "Ruta de los recursos: " << str3 << std::endl;

                std::cout << "Color del vacio: " << clear_color.x << " " 
            									<< clear_color.y << " "
            									<< clear_color.z << " "
            									<< clear_color.w << " "
            									<< std::endl;

                std::cout << "DLL del juego: " << str4 << std::endl;
                std::cout << "Nombre de la ventana: " << str5 << std::endl;
                std::cout << "Ruta del icono: " << str6 << std::endl;

                std::cout << "Ventana: " << width << "x" << height << std::endl;

                std::cout << "Configuracion previa: " << disabled << std::endl;

                core::GameConfigurator::SaveToFile(CONFIGURATOR_PATH);
            }

            //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::EndDisabled();

            if (ImGui::Button("Empezar"))
            {
                core::GameConfigurator::_firstScene = str1;
                core::GameConfigurator::_scenesRoot = str2;
                core::GameConfigurator::_assetsRoot = str3;
                core::GameConfigurator::_gameDLL = str4;
                std::cout << "[CHAVALESEDITOR] Primera escena: " << str1 << std::endl << "[CHAVALESEDITOR] Ruta para escenas: " << str2 << std::endl;

                return false;
            }

            ImGui::Checkbox("Usar configuracion anterior", &disabled);

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
    // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your SDL_AppQuit() function]
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

    wstring s1(L" ExecutableProject_d.exe ");
    wstring fs(std::begin(core::GameConfigurator::_firstScene), std::end(core::GameConfigurator::_firstScene));
    wstring sr(std::begin(core::GameConfigurator::_scenesRoot), std::end(core::GameConfigurator::_scenesRoot));
    s1.append(fs);
    s1.append(L" ");
    s1.append(sr);
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