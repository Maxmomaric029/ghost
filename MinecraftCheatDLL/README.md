# Minecraft Ghost Client (Native x64 DLL) - 1.21.4

Este proyecto es una implementación completa de un cliente interno para Minecraft Java Edition 1.21.4 (Vanilla/Ofuscado).

## Correcciones Técnicas Aplicadas
- **Gestión de Memoria JNI**: Implementación de RAII (`ScopedLocalRef`) para evitar fugas de referencias locales y globales.
- **Proyección ESP Real**: Sistema de `WorldToScreen` basado en matrices de cámara reales obtenidas de la JVM.
- **Aimbot Preciso**: Suavizado circular que evita el "latigazo" al cruzar los 180 grados.
- **Detección Robusta**: Búsqueda de ventana basada en clase `GLFW30` y filtrado de título.
- **Compatibilidad 1.21.4**: Uso de nombres de Mojang/Notch reales para un funcionamiento en clientes ofuscados.

## Requisitos de Compilación
- **Visual Studio 2022** con C++20.
- **JDK 21** instalado (la variable `JAVA_HOME` debe estar configurada).
- **Windows SDK** 10.0+.

## Instrucciones
1. Abre `MinecraftCheatDLL.sln`.
2. Compila en configuración **Release | x64**.
3. Inyecta el archivo `MinecraftCheatDLL.dll` resultante en el proceso `javaw.exe`.

*Nota: Presiona `END` para cerrar el cheat e inyectar de nuevo.*
