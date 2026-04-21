# Guía de Desarrollo: CheatMod Fabric 1.21.4

Este proyecto es una base sólida para un mod de trampas modular en Minecraft 1.21.4.

## Requisitos
- **JDK 21**: Imprescindible para Minecraft 1.21.4.
- **IntelliJ IDEA**: Con el plugin "Minecraft Development" instalado.

## Cómo empezar
1. Abre esta carpeta en IntelliJ IDEA.
2. Espera a que Gradle termine de importar el proyecto.
3. Ejecuta `./gradlew genSources` para generar los mapeos de Yarn.
4. Usa la configuración de ejecución `Minecraft Client` para iniciar el juego.

## Estructura
- `com.ejemplo.cheatmod.core`: Lógica base del sistema de módulos.
- `com.ejemplo.cheatmod.modules`: Implementaciones de ESP, KillAura y BowAimbot.
- `com.ejemplo.cheatmod.mixin`: Inyecciones de código en las clases internas de Minecraft.
- `com.ejemplo.cheatmod.utils`: Utilidades matemáticas y de renderizado.

## Errores Comunes
- **Mixins no cargan**: Verifica que el paquete en `mixins.cheatmod.json` sea exacto.
- **NullPointerException en KillAura**: Asegúrate de que `client.world` y `client.player` no sean nulos antes de actuar.
- **ESP no se ve**: Comprueba que las coordenadas estén siendo proyectadas correctamente al espacio de pantalla (2D).

## Compilación
Para generar el archivo JAR final:
```bash
./gradlew build
```
El archivo estará en `build/libs/`.
