<?php

/**
 * @file Provides template-handling functions.
 *
 * Copied en-masse from Valkyrie.
 */

/**
 * Write a file based on a template.
 */
function skynet_template($template_path, $dest_path, $variables = array(), $mode = 0600, $overwrite = FALSE) {
  $contents = _skynet_render_template(_skynet_load_template($template_path), $variables);
  if (!is_dir(dirname($dest_path))) {
    drush_mkdir(dirname($dest_path));
  }
  if ($overwrite || !file_exists($dest_path)) {
    file_put_contents($dest_path, $contents);
    chmod($dest_path, $mode);
    return TRUE;
  }
  drush_set_error('SKYNET_FILE_EXISTS', 'The specified destination file already exists.');
}

/**
 * Load a template file.
 */
function _skynet_load_template($template_path) {
  if (file_exists($template_path) && is_readable($template_path)) {
    drush_log("Template loaded: $template_path");
    return file_get_contents($template_path);
  }
  drush_set_error('SKYNET_MISSING_TEMPLATE', 'The specified template file is either missing or un-readable.');
}

/**
 * Render template, making variables available from $variables associative array.
 *
 * Copied from Aegir's Provision_Config::render_template().
 */
function _skynet_render_template($template, $variables) {
  drush_errors_off();
  extract($variables, EXTR_SKIP); // Extract the variables to a local namespace
  ob_start(); // Start output buffering
  eval('?>' . $template); // Generate content
  $contents = ob_get_contents(); // Get the contents of the buffer
  ob_end_clean(); // End buffering and discard
  drush_errors_on();
  return $contents; // Return the contents
}
