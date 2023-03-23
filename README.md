# LDPC_Experiments
## Использование
```
mpiexec -n [N] [Path_to_LDPC_Experiments.exe] [p_visible_left] [p_visible_right] [p_left] [p_right] [step] [up_const] [loop_const] [max_down]
```
- `N` - число запускаемых процессов
- `Path_to_LDPC_Experiments.exe` - путь к LDPC_Experiments.exe
- `p_visible_left` - левая граница оси абцисс на нарисованном графике
- `p_visible_right` - правая граница оси абцисс на нарисованном графике
- `p_left` - левая граница оси абцисс, участвующая в вычислениях
- `p_right` - правая граница оси абцисс, участвующая в вычислениях
- `step` - шаг прохода по графику
- `up_const` - количество необходимых декодирований
- `loop_const` - число итераций, используемых для дополнительного уточнения вероятности
- `max_down` - максимальное количество 