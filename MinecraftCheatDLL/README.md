# Minecraft Cheat DLL 1.21.4 (x64)

Este es un proyecto C++ nativo para un ghost client interno de Minecraft Java Edition 1.21.4.

## Requisitos
- **Visual Studio 2022** con C++20.
- **JDK 21** (configurado en la variable de entorno `JAVA_HOME`).
- **Windows SDK** 10 o 11.

## Características
- **Inyección JNI**: Conexión robusta con la JVM.
- **External Overlay**: Ventana Direct2D transparente.
- **ESP**: Cajas 2D y trazadores.
- **Aimbot**: Selección de objetivos y rotaciones suaves.

## Compilación
Abre `MinecraftCheatDLL.sln` y compila en configuración **Release | x64**.
El DLL resultante se encontrará en `x64/Release/MinecraftCheatDLL.dll`.

## Uso
Inyecta el DLL en `javaw.exe` usando un inyector como Extreme Injector o GH Injector. Asegúrate de que Minecraft esté corriendo en modo ventana o ventana sin bordes.
