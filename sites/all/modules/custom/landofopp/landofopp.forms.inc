<?php 
/**
 * @file
 * LandofOpportunity implement search form.
 */
 
/*
 * Impplement custom search form
 * Field keywords implement with autocomlete functions
 * @return array
*/
function loo_search_form($form, &$form_state) {
  
  $form['keywords'] = array(
    '#type' => 'textfield',
    '#autocomplete_path' => 'tags/autocomplete',
    '#maxlength' => 255,
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Search',
  );
  return $form;
}
/**
 * Autocomlete tags name for serach keywords.
 * @param text $string
 * @return json array
*/
function _tags_autocomplete($string) {
  $matches = array();
  $result = db_select('taxonomy_term_data', 't')
    ->fields('t', array('name'))
    ->condition('name', '%' . db_like($string) . '%', 'LIKE')
    ->execute();

  // save the query to matches
  foreach ($result as $row) {
    $keyword = check_plain(drupal_strtolower($row->name));
    $pos = strpos($keyword, $string);
    if ($pos == 0) $matches[$row->name] = $keyword;
  }

  // Return the result to the form in json
  drupal_json_output($matches);
}