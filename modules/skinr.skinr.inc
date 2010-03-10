<?php
// $Id$
/**
 * @file
 * Provide skinr handling for page level rules.
 */

/**
 * @defgroup skinr page rule handlers
 *
 * @{
 */

/**
 * Implementation of hook_skinr_config().
 */
function skinr_skinr_config() {
  $data['page']['form']['skinr_rule_edit'] = array(
    'index_handler' => 'page_skinr_form_index_handler',
    'preprocess_hook' => 'page',
    'title' => t('page rule settings'),
    'skinr_weight' => 0,
    'collapsed' => FALSE,
  );
  $data['page']['form']['skinr_ui_form'] = array(
    'preprocess_hook' => 'page',
    'title' => t('page rule settings'),
    'collapsed' => FALSE,
  );
  $data['page']['preprocess']['html'] = array(
    'index_handler' => 'page_skinr_preprocess_index_handler',
  );
  $data['page']['contextual_links']['html'] = array(
    'contextual_links_handler' => 'page_skinr_contextual_links',
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
function page_skinr_form_index_handler($op, &$form, $form_state) {
  switch ($op) {
    case 'form':
      if (!empty($form['rule']['rid']['#value'])) {
        return $form['rule']['rid']['#value'];
      }
      else {
        return 0;
      }

    case 'submit':
      return $form_state['values']['rid'];
  }
}

/**
 * Skinr preprocess index handler.
 *
 * @param &$vars
 *   Passes in the $vars parameter from module_preprocess().
 * @return
 *   The index where we can find our values in Skinrs data structure. If an
 *   array is returned, it will loop through each index in Skinr's data
 *   structure and merge the returned classes.
 */
function page_skinr_preprocess_index_handler(&$vars) {
  $rules = skinr_rule_load();

  // Find any page level skinr options and return an array of them.
  $indices = array();
  foreach ($rules as $rule) {
    if (skinr_rule_visible($rule->rid)) {
      $indices[] = $rule->rid;
    }
  }
  return $indices;
}

/**
 * Skinr contextual links handler.
 *
 * @param &$variables
 *   Passes in the $variables parameter from skinr_preprocess().
 * @return
 *   An associative array. Each value is an array that forms the function
 *   arguments for menu_contextual_links(). For example:
 *   @code
 *    $links = array(
 *      'skinr-modulename' => array(
 *        'admin/appearance/skinr/edit', array('system', 'navigation')),
 *      ),
 *      'skinr-modulename-1' => array(
 *        'admin/appearance/skinr/edit', array('system', 'something-else')),
 *      ),
 *    );
 *   @endcode
 */
function page_skinr_contextual_links(&$variables) {
  $rules = skinr_rule_load();
  $links = array();
  $counter = 1;

  foreach ($rules as $rule) {
    if (skinr_rule_visible($rule->rid)) {
      $links['skinr-page-' . $counter++] = array(
        'admin/appearance/skinr/edit', array('page', $rule->rid),
      );
    }
  }
  return $links;
}

/**
 * @}
 */
