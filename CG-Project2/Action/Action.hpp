#pragma once

/*
 * This enum is used to tell what Action should do the Engine when something
 * happen in ImGui windows.
 */
enum Action {
    ADD_CUBE_ENTITY,
    ADD_SPHERE_ENTITY,
    ADD_OBJECT_ENTITY,
    ADD_DIRECT_LIGHT,
    ADD_POINT_LIGHT,
    ADD_SPOT_LIGHT,
    DEL_ENTITY,
    DEL_CUSTOM_LIGHT,
    REFRESH_PROJ,
    START_SIM,
    STOP_SIM,
};
