<?php

/**
 * views_attachment_blocks
 */
/**
 * Implementation of hook_views_handlers().
 */
function views_attachment_blocks_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_attachment_blocks'),
    ),
    'handlers' => array(
      'views_attachment_blocks_handler_plugin_page_blocks' => array(
        'parent' => 'views_handler_field_node',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function views_attachment_blocks_views_plugins() {
  return array(
    'module' => 'views', // This just tells our themes are elsewhere.
    'display' => array(
      'attachment_block' => array(
        'parent' => 'attachment',
        'title' => t('Attachment block'),
        'admin' => t('Attachment block'),        
        'handler' => 'views_attachment_blocks_plugin_attachment_block',
        'path' => drupal_get_path('module', 'views_attachment_blocks'),
        'theme' => 'views_view',
        'use ajax' => TRUE,
      ),
    ),
  );
}