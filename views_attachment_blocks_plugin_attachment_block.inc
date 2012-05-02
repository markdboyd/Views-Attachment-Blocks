<?php
// $Id: views_attachment_blocks_plugin_attachment_header_node.inc,v 1.1.2.4 2010/09/21 16:52:19 joachim Exp $

/**
 * The plugin that handles a header node attachment display.
 *
 * A header node attachment display outputs the content of the node found at the
 * path 'VIEWPATH/text'. This allows non-admins to easily edit the effective
 * header or footer text of the view.
 *
 * @todo:
 *  - add a setting for either the complete path, or the path suffix,
 *    or even a node autocomplete selection widget.
 *  - if basing the path on the view's path, send arguments.
 *  - consider using title rather than path??
 *  - pass arguments by validated value: reach into $this->view->argument and 
 *    pass values from that.
 *  - work with prepopulate module to give admins a 'create header for this view'
 *    that fills in the URL path setting automatically.
 */
class views_attachment_blocks_plugin_attachment_block extends views_plugin_display_attachment {
  /**
   * Provide the summary for attachment options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    $options['block'] = array(
      'category' => 'attachment',
      'title' => t('Block'),      
      'value' => $this->get_option('block') ? $this->get_option('block') : 'None',
    );
  }

  function option_definition () {
    $options = parent::option_definition();

    $options['block'] = array('default' => 0);

    return $options;
  }
  
  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);

    $default_theme = variable_get('theme_default', 'bartik');
    $blocks = array();

    $blocks[0] = t('Select a block...');
    foreach ( _block_rehash($default_theme) as $idx => $block ) { 
      $block_key = $block['module'] . '_' . $block['delta'];
      $blocks[$block_key] = $block['info'];
    }    

    switch ($form_state['section']) {
      case 'block':
        $form['#title'] .= t('Block');
        $form['block'] = array(
          '#type' => 'select',
          '#description' => t('The block to render as the content for this attachment.'),
          '#options' => $blocks,
          '#default_value' => $this->get_option('block'),
        );
        break;
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'block':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        break;
    }
  }
  
  /**
   * Attach to another view.
   */
  function attach_to($display_id) {
    $displays = $this->get_option('displays');

    if (empty($displays[$display_id])) {
      return;
    }

    if (!$this->access()) {
      return;
    }

    $attachment = $this->get_block_content();    

    switch ($this->get_option('attachment_position')) {
      case 'before':
        $this->view->attachment_before .= $attachment;
        break;
      case 'after':
        $this->view->attachment_after .= $attachment;
        break;
      case 'both':
        $this->view->attachment_before .= $attachment;
        $this->view->attachment_after  .= $attachment;
        break;
    }
  }  
  
  /**
   * Get the content to show as this view's header.
   */
  function get_block_content() {    
    $block_key = $this->get_option('block');

    if ( $block_key ) {      
      $block_id = explode('_', $block_key);            

      $block = (object)module_invoke($block_id[0], 'block', 'view', $block_id[1]);            
      $block->module = $block_id[0];
      $block->delta = $block_id[1];

      return theme('block', $block);
    }

    return;
  }
}
