# ⚔️ La Aldea en las Sombras
### RPG de texto en C++ — Acto 1 en desarrollo

---

## 📖 Historia

El mejor rey que el mundo había conocido. Noble, valiente, justo. Jamás perdido una batalla.

Hasta que apareció **Angra** — el Dragón del Caos, una entidad cósmica que se rige por la destrucción. El rey Lancelot partió a enfrentarlo acompañado de sus siervos más leales. Pero algunos enemigos no pueden vencerse.

Lancelot cayó. Y Angra, reconociendo el valor de su espíritu, ató su alma para usarlo como cuerpo.

Justo antes de morir, el rey le ordenó al mago: *"Llevatelos de aquí."*

El teletransporte los salvó... pero el aliento del dragón ya los había alcanzado.

Despiertas en una aldea destruida. Sin memoria. Sin saber quién eres ni cómo llegaste. Solo sabes que algo terrible pasó, y que hay un rey que necesita ser salvado.

---

## 🎮 El Juego

**La Aldea en las Sombras** es un RPG de texto por turnos desarrollado en C++. Exploras un mundo corrompido por el caos, combates enemigos en batallas tácticas y avanzas hacia el enfrentamiento final con el Rey Dragón.

### Clases disponibles

| Clase | Atributo principal | Estilo de juego |
|-------|-------------------|-----------------|
| ⚔️ Guerrero | Fuerza | Daño físico, alta resistencia |
| 🔮 Mago | Inteligencia | Daño mágico, efectos de estado |
| 🏹 Cazador | Destreza | Velocidad, críticos, multi-golpe |

Cada clase tiene su propio árbol de habilidades con desbloqueos en nivel 5, 10 y 15 — incluyendo una **subclase** que define tu estilo definitivo de combate.

### Sistemas implementados

- ⚔️ **Combate por turnos** con sistema de velocidad, críticos y efectos de estado
- 🌍 **4 zonas** con enemigos escalados y atmósfera propia
- 🎲 **Sistema de loot** con rareza: Común, Raro y Legendario
- 🧪 **Inventario** de consumibles con pociones y elixires
- 🏪 **Tienda** con compra y venta de equipo
- 💀 **Efectos de estado**: Quemadura, Veneno, Sangrado, Parálisis, Congelación y más
- 🏆 **Reliquias** con efectos pasivos únicos
- 🐉 **Jefe final** con múltiples fases de combate

### Enemigos notables

- **Arzobispo Veldrame** — Un hombre santo corrompido por el caos. Guarda la salida de la primera zona.
- **Nzz'gorah** — Una entidad sin nombre traída de otra dimensión por el aliento de Angra. No es de este mundo.
- **Lancelot, el Rey Dragón** — El jefe final. Tu rey. No vas a vengarte — vas a liberarlo.

---

## 🗺️ Zonas

| Zona | Nombre | Nivel recomendado |
|------|--------|-------------------|
| 1 | Aldea en las Sombras | 1 - 6 |
| 2 | Bosque Oscuro | 7 - 11 |
| 3 | Pantano Siniestro | 12 - 16 |
| 4 | Tierra Desolada | 17 - 20 |
| Final | Castillo Abandonado | 20+ |

---

## 🛠️ Compilar y ejecutar

### Requisitos
- Compilador C++17 o superior (g++ recomendado)
- Windows (usa `system("cls")` y `system("pause")`)

### Compilar con g++
```bash
g++ -std=c++17 -o Reino *.cpp
```

### Ejecutar
```bash
./Reino
```
O usar el archivo `Run.bat` incluido en Windows.

---

## 📋 Estado del proyecto

**Versión actual:** 1.30 — Acto 1 en construcción

| Sistema | Estado |
|---------|--------|
| Motor principal | ✅ Completo |
| Sistema de combate | ✅ Completo |
| Árbol de habilidades | ✅ Completo (hasta nivel 15) |
| Zonas 1-4 + Jefe final | ✅ Completo |
| Arzobispo Veldrame | 🔧 En desarrollo |
| Nzz'gorah | 🔧 Pendiente |
| Habilidades nivel 20 | 🔧 Pendiente |
| Sistema de rareza Elite | 🔧 Pendiente |
| Ítems únicos | 📋 Planeado |
| Imágenes / SFML | 📋 Planeado |

---

## 👤 Autor

Desarrollado por **hidanhell** — apasionado de los RPGs que decidió hacer el suyo propio.
Proyecto personal de aprendizaje en C++. En desarrollo activo.

---

*"Recuerda... soy un ente cósmico. No desaparezco. En algún tiempo, quizás miles de años... estaré de regreso."*
*— Angra, el Dragón del Caos*

