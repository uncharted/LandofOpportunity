<?php 

/**
 * @file
 * LandofOpportunity hooks functions.
 */

/**
 * Implements hook_node_prepare().
 * Added custome js and css for add/edit node
 */
function landofopp_node_prepare($node) {
  $css = drupal_get_path('module', 'landofopp') . '/css/admin.css';
  $js = drupal_get_path('module', 'landofopp') . '/js/admin.js';
  drupal_add_css($css);
  drupal_add_js($js);
}


/**
 * Implements hook_form_FORM_ID_alter().
 * Comment form
 * Change label text, and add own submit function
 */
function landofopp_form_comment_form_alter(&$form, &$form_state, $form_id) {

  $form['actions']['submit']['#value'] = t('Submit');
  
  $form['#submit'][] = 'landofopp_comment_form_submit';
  $form['comment_body']['und'][0]['#base_type'] = 'textfield';
  $form['comment_body']['und'][0]['#title'] = t('Add a comment...');
  
  if (!empty($form['comment_body']['und'][0]['value'])) {
    $form['comment_body']['und'][0]['value']['#title'] = t('Add a comment...');
    $form['comment_body']['und'][0]['value']['#type'] = 'textfield';
  }
  
  // HACK, stop ctools from modifying us in node_comment_form.inc
  $form_state['ctools comment alter'] = FALSE;
}

/**
 * Return json output from new comment and refresh all comments from node
 * Use for ajax submit in player video.
 * @param $form
 * @param $form_state
 * @return array
*/
function landofopp_comment_form_submit($form, &$form_state) {
  $message = t('Your comment has been submitted and is awaiting moderation');
  $item = $form_state['comment'];
  if (!empty($_POST['is_ajax'])) {
    $seconds = landofopp_get_field_value($item, 'field_time');
    
    $body = landofopp_get_field_value($item, 'comment_body', 'value');
    $body = strip_tags($body, '<strong><em><u>');
    $body = check_plain($body);
    $body = str_replace('&amp;nbsp;', ' ', $body);
    $body = str_replace('&quot;', '"', $body);

    if ($seconds < 3600) {
      $time = _hms_seconds_to_formatted($seconds, 'm:ss');
    } 
    else $time = _hms_seconds_to_formatted($seconds, 'h:mm:ss');
    // create new commnent
    $comment = array(
      'cid' => $item->cid,
      'seconds' => $seconds,
      'time' => $time,
      'body' => $body,
      'author' => $item->name,
    );
    
    $nid = $item->nid;
    $node = node_load($nid);
    // get all comments from node
    $comments = landofopp_get_comment_from_richmedia($node);
    
    $results['comment'] = $comment;
    $results['comments'] = $comments;
    
    print drupal_json_output($results);
    die();
  }
  
}
/**
 * Implements hook_form_alter().
 * Changed form content for form : user register, user login.
 * Hide user partner for register form.
 * After success login user redirect to content overview page.
 */
function landofopp_form_alter(&$form, &$form_state, $form_id) {
  if ($form_id === 'user_register_form') {
    // hide autoassignrole_user
    $form['autoassignrole_user']['#access'] = FALSE;
    if (arg(0) == 'user') {
      $form['field_user_partner']['#access'] = FALSE;
    }
    $form['actions']['submit']['#suffix'] = '<p>Already have an account? ' . l(t('Login'), 'user') . '</p>';
  }
  if ($form_id === 'user_login') {
    $form['#action'] = $form['#action'] . '?destination=admin/content';
    $form['actions']['submit']['#suffix'] = '<p>Don’t have an account? ' . l(t('Create one now'), 'user/register') . '</p>';
    $form['name']['#description'] = 'Don’t have a username? ' . l(t('Create a new account'), 'user/register');
    $form['pass']['#description'] = l(t('Forgot your password?'), 'user/password');
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 * Form add/edit for category node
 * Remove sticky checkbox for category node
 * 
*/
function landofopp_form_category_node_form_alter(&$form, &$form_state, $form_id) {
  unset($form['options']['sticky']);
}
/**
* Implements hook_form_FORM_ID_alter().
* Form add/edit for  event node
* 
*/
function landofopp_form_event_node_form_alter(&$form, &$form_state, $form_id) {
  unset($form['options']['sticky']);
  drupal_add_library('system', 'ui.slider');
  $js = drupal_get_path('module', 'landofopp') . '/js/timeline.js';
  drupal_add_js($js);
  $timeline_headers = '';
  // vocabulary ID 
  $vid = 3;
  $terms = taxonomy_get_tree($vid);
  foreach ( $terms as $term ) {
    $timeline_headers .= '<div class="sector"><h3>' . $term->name . '</h3></div>';
  }
  $form['#prefix'] = '<div class="headers">' . $timeline_headers . '</div>';

  //hide preview button
  $form['actions']['preview']['#access'] = FALSE;
}

/*
 * 
 * Implements hook_entity_info_alter().
 * Add new view mode "featured" and "timeline", it used on RMV content type 
 * @see node-rich_media.tpl.php
 */
function landofopp_entity_info_alter(&$entity_info) {

  $entity_info['node']['view modes']['featured'] = array(
    'label' => t('Featured'),
    'custom settings' => 1
  );

  $entity_info['node']['view modes']['timeline'] = array(
    'label' => t('Timeline'),
    'custom settings' => 1
  );
  
}