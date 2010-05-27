<?php
// $Id$

/**
 * @file
 * Admin page callbacks for the skinr module.
 */

/**
 * Implements hook_skinr_operations().
 */
function skinr_ui_skinr_operations() {
  $operations = array(
    'delete' => array(
      'label' => t('Delete selected Skinr settings'),
      'callback' => NULL,
    ),
  );
  return $operations;
}

/**
 * List skinr administration filters that can be applied.
 */
function skinr_ui_filters() {
  // Theme filter.
  $themes = list_themes();
  ksort($themes);

  $options = array('[any]' => t('any'));
  foreach ($themes as $theme) {
    if (!$theme->status) {
      continue;
    }
    $options[$theme->name] = $theme->info['name'];
  }

  $filters['theme'] = array(
    'title' => t('theme'),
    'options' => $options,
  );

  // Type filter.
  $config = skinr_fetch_config();

  $options = array('[any]' => t('any'));
  foreach ($config as $type => $data) {
    $options[$type] = $type;
  }

  $filters['type'] = array(
    'title' => t('type'),
    'options' => $options,
  );

  return $filters;
}

/**
 * Return form for skinr administration filters.
 */
function skinr_ui_filter_form() {
  $session = isset($_SESSION['skinr_ui_overview_filter']) ? $_SESSION['skinr_ui_overview_filter'] : array();
  $filters = skinr_ui_filters();

  $i = 0;
  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Show only items where'),
    '#theme' => 'skinr_ui_filters',
  );
  foreach ($session as $filter) {
    list($type, $value) = $filter;
    $value = $filters[$type]['options'][$value];

    if ($i++) {
      $form['filters']['current'][] = array('#markup' => t('<em>and</em> where <strong>%type</strong> is <strong>%value</strong>', array('%type' => $filters[$type]['title'], '%value' => $value)));
    }
    else {
      $form['filters']['current'][] = array('#markup' => t('<strong>%type</strong> is <strong>%value</strong>', array('%type' => $filters[$type]['title'], '%value' => $value)));
    }
    if (in_array($type, array('theme', 'type'))) {
      // Remove the option if it is already being filtered on.
      unset($filters[$type]);
    }
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    $form['filters']['status'][$key] = array(
      '#type' => 'select',
      '#options' => $filter['options'],
      '#title' => $filter['title'],
      '#default_value' => '[any]',
    );
  }

  $form['filters']['actions'] = array(
    '#type' => 'actions',
    '#id' => 'skinr-ui-admin-buttons',
    '#attributes' => array('class' => array('container-inline')),
  );
  if (count($filters)) {
    $form['filters']['actions']['submit'] = array('#type' => 'submit', '#value' => (count($session) ? t('Refine') : t('Filter')));
  }
  if (count($session)) {
    $form['filters']['actions']['undo'] = array('#type' => 'submit', '#value' => t('Undo'));
    $form['filters']['actions']['reset'] = array('#type' => 'submit', '#value' => t('Reset'));
  }

  drupal_add_js('misc/form.js');

  return $form;
}

/**
 * Returns HTML for a node administration filter selector.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_skinr_ui_filters($variables) {
  $form = $variables['form'];
  $output = '';

  $output .= '<ul class="clearfix">';
  if (!empty($form['current'])) {
    foreach (element_children($form['current']) as $key) {
      $output .= '<li>' . drupal_render($form['current'][$key]) . '</li>';
    }
  }
  $output .= '</ul>';

  $output .= '<div class="clearfix">';

  if (!empty($form['status']) && count(element_children($form['status']))) {
    $output .= '<dl class="multiselect">' . (!empty($form['current']) ? '<dt><em>' . t('and') . '</em> ' . t('where') . '</dt>' : '');
  
    $output .= '<dd>';
  
    foreach (element_children($form['status']) as $key) {
      $output .= drupal_render($form['status'][$key]);
    }
    $output .= '</dd>';
  
    $output .= '</dl>';
  }
  $output .= drupal_render($form['actions']);

  $output .= '</div>';

  return $output;
}

/**
 * Process result from node administration filter form.
 */
function skinr_ui_filter_form_submit($form, &$form_state) {
  $filters = skinr_ui_filters();
  switch ($form_state['values']['op']) {
    case t('Filter'):
    case t('Refine'):
      // Apply every filter that has a choice selected other than 'any'.
      foreach ($filters as $filter => $options) {
        if (isset($form_state['values'][$filter]) && $form_state['values'][$filter] != '[any]') {
          // Flatten the options array to accommodate hierarchical/nested options.
          $flat_options = form_options_flatten($filters[$filter]['options']);
          // Only accept valid selections offered on the dropdown, block bad input.
          if (isset($flat_options[$form_state['values'][$filter]])) {
            $_SESSION['skinr_ui_overview_filter'][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION['skinr_ui_overview_filter']);
      break;
    case t('Reset'):
      $_SESSION['skinr_ui_overview_filter'] = array();
      break;
  }
}

/**
 * Menu callback; displays the skinr listing page.
 */
function skinr_ui_list($form, &$form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return skinr_ui_multiple_delete_confirm($form, $form_state, array_filter($form_state['values']['skins']));
  }
  $form['filter'] = skinr_ui_filter_form();
  $form['#submit'][] = 'skinr_ui_filter_form_submit';
  $form['admin'] = skinr_ui_admin_skins();
  
  drupal_add_css(drupal_get_path('module', 'skinr') . '/css/skinr_ui.css');

  return $form;
}

/**
 * Form builder: Builds the node administration overview.
 */
function skinr_ui_admin_skins() {
  $session = !empty($_SESSION['skinr_ui_overview_filter']) && is_array($_SESSION['skinr_ui_overview_filter']) ? $_SESSION['skinr_ui_overview_filter'] : array();
  $filters = array();
  foreach ($session as $filter) {
    list($key, $value) = $filter;
    $filters[$key] = $value;
  }

  // Build an 'Update options' form.
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#attributes' => array('class' => array('container-inline')),
  );

  $options = array();
  $options['delete'] = t('Delete the selected Skinr settings');

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'publish',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#validate' => array('skinr_ui_admin_skins_validate'),
    '#submit' => array('skinr_ui_admin_skins_submit'),
  );

  $sorts = array();

  $headers = array(
    'theme' => array('data' => t('Theme'), 'field' => 'theme'),
    'type' => array('data' => t('Type'), 'field' => 'type'),
    'sid' => array('data' => t('Skinr ID'), 'field' => 'sid'),
    'operations' => array('data' => t('Operations')),
  );

  $ts = tablesort_init($headers);
  $sort = SORT_DESC;
  if ($ts['sort'] == 'asc') {
    $sort = SORT_ASC;
  }

  switch ($ts['sql']) {
    case 'type':
    case 'sid':
      $sortby = $ts['sql'];
      break;

    case 'theme':
    default:
      $sortby = 'theme';
      break;
  }

  $themes = list_themes();
  ksort($themes);

  $options = array();
  $destination = drupal_get_destination();
  $type = !empty($filters['type']) ? $filters['type'] : NULL;

  foreach ($themes as $theme) {
    if (!$theme->status || !empty($theme->info['hidden']) || (!empty($filters['theme']) && $filters['theme'] != $theme->name)) {
      continue;
    }
    if (empty($type)) {
      $skinrs = skinr_get($theme->name);
    }
    else {
      $skinrs = array($type => skinr_get($theme->name, $type));
    }

    foreach ($skinrs as $module => $elements) {
      foreach ($elements as $sid => $skin) {
        $operations = array(
          'edit' => array(
            'title' => t('edit'),
            'href' => 'admin/appearance/skinr/edit/nojs/' . $module . '/' . $sid,
            'query' => $destination,
          ),
          'delete' => array(
            'title' => t('delete'),
            'href' => 'admin/appearance/skinr/delete/'. $theme->name .'/'. $module .'/'. $sid,
            'query' => $destination,
          ),
        );
        $options[$theme->name . '__' . $module . '__' . $sid] = array(
          'theme' => $theme->info['name'],
          'type' => $module,
          'sid' => $sid,
          'operations' => array(
            'data' => array(
              '#theme' => 'links__skinr_operations',
              '#links' => $operations,
              '#attributes' => array('class' => array('links', 'inline')),
            ),
          ),
        );
        $sorts[] = strip_tags($options[$theme->name . '__' . $module . '__' . $sid][$sortby]);
      }
    }
  }
  
  // Sort table.
  array_multisort($sorts, $sort, $options);

  // Output table.
  $form['skins'] = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $options,
    '#empty' => t('No content available.'),
  );
  $form['pager'] = array('#markup' => theme('pager', array('tags' => NULL)));

  return $form;
}

/**
 * Validate skinr_ui_list form submissions.
 *
 * Check if any skinr settings have been selected to perform the chosen
 * 'Update option' on.
 */
function skinr_ui_admin_skins_validate($form, &$form_state) {
  // Error if there are no items to select.
  if (!is_array($form_state['values']['skins']) || !count(array_filter($form_state['values']['skins']))) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process skinr_ui_list form submissions.
 *
 * Execute the chosen 'Update option' on the selected skinr settings.
 */
function skinr_ui_admin_skins_submit($form, &$form_state) {
  $operations = module_invoke_all('skinr_operations');
  $operation = $operations[$form_state['values']['operation']];
  // Filter out unchecked nodes
  $nodes = array_filter($form_state['values']['skins']);
  if ($function = $operation['callback']) {
    // Add in callback arguments if present.
    if (isset($operation['callback arguments'])) {
      $args = array_merge(array($nodes), $operation['callback arguments']);
    }
    else {
      $args = array($nodes);
    }
    call_user_func_array($function, $args);

    cache_clear_all();
  }
  else {
    // We need to rebuild the form to go to a second step. For example, to
    // show the confirmation form for the deletion of nodes.
    $form_state['rebuild'] = TRUE;
  }
}

function skinr_ui_multiple_delete_confirm($form, &$form_state, $skins) {
  $themes = list_themes();

  $form['skins'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($skins as $skin => $value) {
    $parts = explode('__', $skin, 3);
    $form['skins'][$skin] = array(
      '#type' => 'hidden',
      '#value' => $skin,
      '#prefix' => '<li>',
      '#suffix' => t('Skinr ID %sid of type %type for theme %theme', array('%sid' => $parts[2], '%type' => $parts[1], '%theme' => $themes[$parts[0]]->info['name'])) ."</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'skinr_ui_multiple_delete_confirm_submit';
  $confirm_question = format_plural(count($skins),
                                  'Are you sure you want to delete this item?',
                                  'Are you sure you want to delete these items?');
  return confirm_form($form,
                    $confirm_question,
                    'admin/appearance/skinr', t('This action cannot be undone.'),
                    t('Delete'), t('Cancel'));
}

function skinr_ui_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['skins'] as $skin => $value) {
      $parts = explode('__', $skin, 3);

      $skinr = new StdClass();
      $skinr->theme = $parts[0];
      $skinr->module = $parts[1];
      $skinr->sid = $parts[2];
      $skinr->skins = array();

      skinr_set($skinr);
    }
    
    $count = count($form_state['values']['skins']);
    if ($count == 1) {
      watchdog('content', 'Deleted 1 skinr setting.');
    }
    else {
      watchdog('content', 'Deleted @count skinr settings.', array('@count' => $count));
    }
    drupal_set_message(format_plural($count, 'Deleted 1 skinr setting.', 'Deleted @count skinr settings.'));
  }
  $form_state['redirect'] = 'admin/appearance/skinr';
}

/**
 * Menu callback; displays the skinr settings administration form.
 */
function skinr_ui_settings($form, &$form_state) {
  $form = array();

  $form['overlay'] = array(
    '#type' => 'fieldset',
    '#title' => t('Overlay'),
    '#collapsible' => TRUE,
  );
  $form['overlay']['skinr_overlay_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Window width'),
    '#default_value' => variable_get('skinr_overlay_width', 600),
    '#description' => t('The width of the overlay window. Leave this field blank to allow the window to automatically resize itself to a minimal width.'),
  );
  $form['overlay']['skinr_overlay_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Window height'),
    '#default_value' => variable_get('skinr_overlay_height', NULL),
    '#description' => t('The height of the overlay window. Leave this field blank to allow the window to automatically resize itself to the height of your browser.'),
  );

  return system_settings_form($form);
}

/**
 * Validation handler for skinr settings administration form.
 */
function skinr_ui_settings_validate(&$form, &$form_state) {
  if (empty($form_state['values']['skinr_overlay_width'])) {
    $form_state['values']['skinr_overlay_width'] = NULL;
  }
  if (empty($form_state['values']['skinr_overlay_height'])) {
    $form_state['values']['skinr_overlay_height'] = NULL;
  }
}

/**
 * Menu callback; displays Skinr settings export form.
 */
function skinr_ui_export_form($form, &$form_state, $theme = NULL) {
  $form = array();
  $themes = list_themes();

  if ($theme) {
    // Export an individual theme.
    $theme = str_replace('-', '_', $theme);
    $skinrs = skinr_get($theme);

    // Convert classes to arrays.
    foreach ($skinrs as $module => $data) {
      foreach ($data as $sid => $skinr) {
        $skinrs[$module][$sid] = (array) $skinrs[$module][$sid];
      }
    }

    $code = '$skinrs = '. var_export($skinrs, TRUE) .';';
    $lines = substr_count($code, "\n") + 1;

    $form['theme'] = array(
      '#type' => 'textfield',
      '#title' => t('Theme'),
      '#value' => $themes[$theme]->info['name'],
      '#disabled' => TRUE,
    );

    $form['skinr_settings'] = array(
      '#type' => 'textarea',
      '#title' => t('Skinr settings'),
      '#default_value' => $code,
      '#rows' => min($lines, 150),
    );
  }
  else {
    // Give the option for which theme to export.

    $options = array();
    ksort($themes);
    $current_theme = skinr_current_theme(TRUE);

    // Put default theme at the top.
    $options[$current_theme] = $themes[$current_theme]->info['name']. ' [' . t('default') . ']';

    foreach ($themes as $theme) {
      if (!empty($theme->info['hidden'])) {
        continue;
      }
      
      if ($theme->name == $current_theme) {
        // Do nothing.
      }
      elseif ($theme->status) {
        $options[$theme->name] = $theme->info['name'] . ' [' . t('enabled') . ']';
      }
      else {
        $options[$theme->name] = $theme->info['name'];
      }
    }

    $form['theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#description' => t('Theme to export the skinr settings for.'),
      '#options' => $options,
      '#required' => TRUE,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Export'),
    );
  }

  return $form;
}

/**
 * Validation handler for skinr settings export form.
 */
function skinr_ui_export_form_validate(&$form, &$form_state) {
  if (!empty($form_state['values']['theme']) && preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['theme'])) {
    form_error($form['theme'], t('The theme name must be alphanumeric and can contain underscores only.'));
  }
}

/**
 * Submit handler for skinr settings export form.
 */
function skinr_ui_export_form_submit(&$form, &$form_state) {
  drupal_goto('admin/appearance/skinr/export/' . str_replace('_', '-', $form_state['values']['theme']));
}

/**
 * Menu callback; displays Skinr settings import form.
 */
function skinr_ui_import_form($form, &$form_state) {
  $form = array();

  $options = array();
  $themes = list_themes();
  ksort($themes);
  $current_theme = skinr_current_theme(TRUE);

  // Put default theme at the top.
  $options[$current_theme] = $themes[$current_theme]->info['name']. ' [' . t('default') . ']';

  foreach ($themes as $theme) {
    if (!empty($theme->info['hidden'])) {
      continue;
    }
    if ($theme->name == $current_theme) {
      // Do nothing.
    }
    elseif ($theme->status) {
      $options[$theme->name] = $theme->info['name'] . ' [' . t('enabled') . ']';
    }
    else {
      $options[$theme->name] = $theme->info['name'];
    }
  }

  $form['theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#options' => $options,
    '#required' => TRUE,
  );
  $form['skinr_settings'] = array(
    '#type' => 'textarea',
    '#title' => t('Skinr settings'),
    '#description' => t('Paste skinr settings here.'),
    '#rows' => 16,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Validation handler for Skinr settings import form.
 */
function skinr_ui_import_form_validate(&$form, &$form_state) {
  if (empty($form_state['values']['skinr_settings'])) {
    // Error.
    form_error($form['skinr_settings'], t('These are not valid Skinr settings.'));
    return;
  }

  $skinrs = '';
  ob_start();
  eval($form_state['values']['skinr_settings']);
  ob_end_clean();

  foreach ($skinrs as $module => $ignored) {
    if (!is_array($skinrs[$module])) {
      // Error.
      form_error($form['skinr_settings'], t('These are not valid Skinr settings.'));
    }
    foreach ($skinrs[$module] as $sid => $ignored) {
      if (!is_array($skinrs[$module][$sid])) {
        // Error.
        form_error($form['skinr_settings'], t('These are not valid Skinr settings.'));
      }

      // Cast skinr array to object so we can actually use it.
      $skinrs[$module][$sid] = (object) $skinrs[$module][$sid];
      if (!skinr_validate($skinrs[$module][$sid])) {
        // Error.
        form_error($form['skinr_settings'], t('These are not valid Skinr settings.'));
      }
    }
  }

  if (!empty($form_state['values']['theme']) && preg_match('/[^a-zA-Z0-9_]/', $form_state['values']['theme'])) {
    form_error($form['theme'], t('The theme name must be alphanumeric and can contain underscores only.'));
  }

  $form_state['skinrs'] = &$skinrs;
}

/**
 * Submit handler for skinr settings import form.
 */
function skinr_ui_import_form_submit(&$form, &$form_state) {
  foreach ($form_state['skinrs'] as $module => $skinrs) {
    foreach ($skinrs as $sid => $skinr) {
      if (!skinr_set($skinr)) {
        drupal_set_message(t('Skinr settings for the theme %theme could not be saved!', array('%theme' => $form_state['values']['theme'])), 'error');
      }
    }
  }

  drupal_set_message(t('Skinr settings for the theme %theme have been saved.', array('%theme' => $form_state['values']['theme'])));
  drupal_goto('admin/appearance/skinr');
}

/**
 * Menu callback; displays the delete confirmation for a skinr page rule.
 */
function skinr_ui_delete_confirm($form, &$form_state, $theme, $module, $sid) {
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => isset($form_state['theme']) ? $form_state['theme'] : $theme,
  );
  $form['module'] = array(
    '#type' => 'value',
    '#value' => isset($form_state['module']) ? $form_state['module'] : $module,
  );
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => isset($form_state['sid']) ? $form_state['sid'] : $sid,
  );

  $themes = list_themes();

  return confirm_form($form,
    t('Are you sure you want to delete these Skinr settings?'),
    isset($_GET['destination']) ? $_GET['destination'] : 'admin/appearance/skinr',
    t('This action cannot be undone.<br /> Theme: !theme<br />Module: !module<br />Skinr ID: !sid', array('!theme' => $themes[$theme]->info['name'], '!module' => $module, '!sid' => $sid)),
    t('Delete'),
    t('Cancel')
  );
}

function skinr_ui_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $skinr = new stdClass();
    $skinr->theme = $form_state['values']['theme'];
    $skinr->module = $form_state['values']['module'];
    $skinr->sid = $form_state['values']['sid'];
    $skinr->skins = array();
    $skinr->settings = array();

    skinr_set($skinr);
  }

  $form_state['redirect'] = 'admin/appearance/skinr';
}
