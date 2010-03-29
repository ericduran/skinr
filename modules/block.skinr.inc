<?php
// $Id$
/**
 * @file
 * Provide skinr handling for block.module
 */

/**
 * @defgroup skinr_block_module block.module handlers
 *
 * @{
 */

/**
 * Implementation of hook_skinr_config().
 */
function block_skinr_config() {
  $data['block']['form']['block_admin_configure'] = array(
    'index_handler' => 'block_skinr_form_index_handler',
    'preprocess_hook' => 'block',
    'title' => t('block settings'),
    'skinr_weight' => 0,
    'collapsed' => FALSE,
  );
  $data['block']['form']['block_add_block_form'] = array(
    'index_handler' => 'block_skinr_form_index_handler',
    'preprocess_hook' => 'block',
    'title' => t('block settings'),
    'skinr_weight' => 0,
    'collapsed' => FALSE,
  );
  $data['block']['form']['skinr_ui_form'] = array(
    'preprocess_hook' => 'block',
    'title' => t('block settings'),
    'collapsed' => FALSE,
  );
  $data['block']['preprocess']['block'] = array(
    'index_handler' => 'block_skinr_preprocess_index_handler',
  );
  $data['block']['contextual_links']['block'] = array(
    'contextual_links_handler' => 'block_skinr_contextual_links',
  );

  return $data;
}

/**
 * Skinr form index handler.
 *
 * @param $op
 *   What kind of action is being performed. Possible values:
 *   - "form": the form elements for Skinr are being inserted in a form
 *   - "submit": the form has been submitted.
 * @param &$form
 *   - For "form", passes in the $form parameter from hook_form_alter().
 *   - For "submit", passes in the $form parameter from hook_form_submit().
 * @param $form_state
 *   - For "form", passes in the $form_state parameter from hook_form_alter().
 *   - For "submit", passes in the $form_state parameter from hook_form_submit().
 * @return
 *   The index where we can find our values in Skinrs data structure.
 */
function block_skinr_form_index_handler($op, &$form, $form_state) {
  switch ($op) {
    case 'form':
      return $form['module']['#value'] . '-' . $form['delta']['#value'];

    case 'submit':
      $form_id = $form['form_id']['#value'];
      if ($form_id == 'block_add_block_form') {
        // This is a new block, so we need to fetch the delta from DB
        $delta = db_query("SELECT delta FROM {blocks} WHERE bid = :bid", array(
          ':bid' => db_last_insert_id('boxes', 'bid'),
        ))->fetchField();
        if ($delta) {
          return $form_state['values']['module'] . '-' . $delta;
        }
      }
      else {
        return $form_state['values']['module'] . '-' . $form_state['values']['delta'];
      }
  }
}

/**
 * Skinr preprocess index handler.
 *
 * @param &$variables
 *   Passes in the $variables parameter from module_preprocess().
 * @return
 *   The index where we can find our values in Skinrs data structure. If an
 *   array is returned, it will loop through each index in Skinr's data
 *   structure and merge the returned classes.
 */
function block_skinr_preprocess_index_handler(&$variables) {
  return $variables['block']->module . '-' . $variables['block']->delta;
}

/**
 * Skinr contextual links handler.
 *
 * @param &$variables
 *   Passes in the $variables parameter from skinr_preprocess().
 * @return
 *   An array. Each value is an array that forms the function arguments for
 *   menu_contextual_links(). For example:
 *   @code
 *     array(
 *       'admin/appearance/skinr/edit', array('system', 'navigation')),
 *     )
 *   @endcode
 */
function block_skinr_contextual_links(&$variables) {
  $links = array();
  $links['skinr-block'] = array(
    'admin/appearance/skinr/edit', array('block', $variables['block']->module . '-' . $variables['block']->delta),
  );
  return $links;
}

/**
 * @}
 */
