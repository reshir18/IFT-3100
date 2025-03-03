/*****************************************************
* TP IFT3100H24 - Knight Maker
 * by Team 12
 *****************************************************
 *
 * ofCustomCamera class implementation
 *
 *****************************************************/
#include "ofCustomCamera.h"

#include <ofAppRunner.h>
#include <ofGraphicsBaseTypes.h>


/**
 * Get ortho zoom
 */
float ofCustomCamera::getOrthoZoom() {
    return m_orthoZoom;
}


/**
 * Set ortho zoom
 */
void ofCustomCamera::setOrthoZoom(float p_zoom) {
    if (p_zoom < 0.1f) {
        p_zoom = 0.1f;
    }
    m_orthoZoom = p_zoom;
}

/**
 * Enable ortho mode
 */
void ofCustomCamera::enableOrtho() {

    ofCamera::enableOrtho();
}

/**
* Apply matrices
*/
void ofCustomCamera::begin() {

    auto viewport = getRenderer()->getCurrentViewport();
    auto renderer = ofGetCurrentRenderer();

    renderer->bind(*this,viewport);

    if (getOrtho()) {
        renderer->translate(getPosition());
        renderer->scale(m_orthoZoom, m_orthoZoom,m_orthoZoom);
        renderer->translate(-getPosition());
    }
}
