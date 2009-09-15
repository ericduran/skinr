// $Id$

# About

Skinr's main purpose is to allow the theme to define a set of reusable and modular CSS styles, and to make those styles available in Drupal's UI . Skinr was developed for themers to allow them to tap into the power of Drupal's modularity and apply those same principals to theme development. It does not provide any styles of it's own. These styles are defined in the .info file of the theme (or subtheme), by the themer and end up in various places in Drupal's UI, such as:

* Block Configuration
* Node Type (and Comment) Configuration
* [Panel](http://drupal.org/project/panels) Panes
* [Views](http://drupal.org/project/views) Displays

One important thing to note is that Skinr is really just a tool. It can be used for different purposes, i.e. a contrib theme, or client theme. Both serve very different purposes and should probably be approached differently with the end user and overall project goals in mind.

Using it to its full potential is really up to you.

If you are looking to implement Skinr on one of your projects, I highly recommend planning it out at the very beginning of the project. Converting an existing theme to use Skinr is generally not recommended if you are under time constraints, UNLESS it is a contributed theme.

# Reporting Issues and Contributing to the project

Please report all issues to the project [issue queue](http://drupal.org/project/issues/skinr).  Make sure you search existing issues first to see if your problem has already been reported. Remember to include as much information as possible about the issue your are experiencing and be patient waiting for a reply. If you are looking to submit a patch, have an idea for a new feature, or want to help with the future plans for Skinr, we need your help. Please start by opening a new issue stating your intentions.

# Sharing your work with others

One of the things we have planned for Skinr in the near future is offer an external site where we can offer packaged themer & designer contributed Skins to the community without having to support an entire contrib theme and dealing with CVS, as this can be a burden for some people. Many themers work on sites for clients where themes are proprietary and cannot be legally shared. By breaking down the contribution to a much smaller size, and with a few tweaks to colors and styles, small bits of great code can be shared.

Please consider taking the time to contribute a nice menu, block, tab style (or whatever). Your contribution could really help out someone new, and we need your help. If you have a skin to share, please e-mail it to jacine@graviteklabs.com. IMPORTANT: You do not have to contribute code. If you are a designer, feel free to submit Photoshop mockups!

# Showing off your work

We love to see how people are using Skinr.  If you have done something cool with Skinr, and want to tell us about it, please e-mail me: jacine@graviteklabs.com

# Documentation

## 1. System Name and [title]

Each skin begins with the prefix "skinr" in your .info file. This allows Drupal and Skinr to easily identify and access information about your skins.

1. Start with a skin name which will be used as the system name. You can name a skin whatever you want. The system name will be used to store information about your skin, including where that skin is applied throughout Drupal. It will not show up in the UI or in any markup. skinr[skin_system_name] will be used to prefix all information about a given skin in your .info file.

2. Next give the Skin a title. This title will be the name of the Skin and will appear in the UI as the form element label, so it should be descriptive of what the Skin is/does.

skinr[skin_system_name][title] = Title

## 2. Widget Type [type]

There are 3 different options for displaying your Skinr styles in the UI. These **types** correspond to Drupal's Form API widgets. They were created to allow you to better present your skins in the UI and to allow fine-grained control over the options you provide.

* _checkboxes_: Can be used for single or multiple options. Do not use "checkbox" as it will not work.
* _select_:  Good for presenting many options when only one can be selected. Note: Multiple selects are not supported. Checkboxes should be used instead.
* _radios_: Can be used for single options.

Syntax:

skinr[skin_system_name][type] = checkboxes
skinr[skin_system_name][type] = radios
skinr[skin_system_name][type] = select

*Note: Skinr applies checkboxes by default so it's not necessary to specify if that's what you want to use.*

## 3. Widget Description [description]

skinr[skin_system_name][description] = This should be a description of my Skin. It will show in the UI just as regular form item descriptions do. It can contain <acronym title="Extensible HyperText Markup Language">XHTML</acronym>, so feel free to be creative, but don't go too crazy.

## 4. Widget Options [options]

*Note: Skinr will include a <none> option for each so they can be unselected by default so there is no need to include a none option here.*
  
### 1. Singular

For skins where only one option should be chosen by the user, the syntax would be as follows:

* If you want to display it in a checkboxes widget:

skinr[skin_system_name][title] = Title
skinr[skin_system_name][description] = Description
skinr[skin_system_name][options][1][label] = Skin option label
skinr[skin_system_name][options][1][class] = skin-option-class

*Note: [type] is not specified in the above example since it is the default provided by Skinr.*

* If you want to display it in a radio widget:

skinr[skin_system_name][title] = Title
skinr[skin_system_name][type] = radios
skinr[skin_system_name][description] = Description
skinr[skin_system_name][options][1][label] = Skin option label
skinr[skin_system_name][options][1][class] = skin-option-class

### 2. Multiple

For skins where multiple options can be selected, the syntax would be as follows:

* If you want to display it in a checkboxes widget:

skinr[skin_system_name][title] = Title
skinr[skin_system_name][description] = Description
skinr[skin_system_name][options][1][label] = Skin option label 1
skinr[skin_system_name][options][1][class] = skin-option-class-1
skinr[skin_system_name][options][2][label] = Skin option label 2
skinr[skin_system_name][options][2][class] = skin-option-class-2

*Note: [type] is not specified in the above example since it is the default provided by Skinr.*

* If you want to display it in a select widget:

skinr[skin_system_name][title] = Title
skinr[skin_system_name][type] = select
skinr[skin_system_name][description] = Description
skinr[skin_system_name][options][1][label] = Skin option label
skinr[skin_system_name][options][1][class] = skin-option-class
skinr[skin_system_name][options][2][label] = Skin option label 2
skinr[skin_system_name][options][2][class] = skin-option-class-2

## 5. Selectively restricting styles to de-clutter the UI using the [features] option:

By default Skinr will assume that your styles can be used anywhere Skinr is available on your site through any supported Drupal feature, i.e. nodes, blocks, etc. If you want to reduce UI clutter and improve the UX you can specify hooks/feature names where your skins should appear within Drupal. [features] do NOT need to be specified. If nothing is specified in your skin it will appear everywhere by default. The feature name is the equivalent of the drupal hook, and may only contain underscores.  Some example hooks:

skinr[skin_system_name][features][] = block
skinr[skin_system_name][features][] = panels_pane
skinr[skin_system_name][features][] = node
skinr[skin_system_name][features][] = comment_wrapper
skinr[skin_system_name][features][] = view

Example Use Case: You need to implement 2 designs for menu lists and the structure is mainly the same, but there are 2 different color options. Your style only includes CSS for menus. You know that Drupal only outputs menus through blocks and panel panes, so you don't want show the style in the skin in views, node types or comments. One way you could do that is:

1. Create the structural CSS in .menu-style
2. Handle each color option in separate classes as extensions of .menu-style, i.e. .menu-white, .menu-black
3. Implement it with Skinr as follows:

skinr[menu_style][title] = Menu Styles
skinr[menu_style][type] = select
skinr[menu_style][description] = Choose either black or white. This style can be used on menus and uniform lists.
skinr[menu_style][features][] = block
skinr[menu_style][features][] = panels_pane
skinr[menu_style][options][1][label] = White
skinr[menu_style][options][1][class] = menu-style menu-white
skinr[menu_style][options][2][label] = Black
skinr[menu_style][options][2][class] = menu-style menu-black

The [features] as specified above tell Skinr that the [menu_style] skin should only be shown in Drupal's UI if the user is editing either a block or a panel pane.

## 6. Adding Javascript and CSS files to Skins

The latest version of Skinr supports the option to associate CSS and Javascript files with a given skin. This new feature allows you to basically package any skin as a fully functional snippet of code. It also aids you with selectively load JS/CSS where necessary. To extend upon the above menu example in #5, lets say that the menu is a drop-down menu and requires some Javascript to properly function, as well as some external CSS files.  Let's also say you also want to support a vertical and horizontal option.

skinr[menu_style][title] = Menu Styles
skinr[menu_style][type] = select
skinr[menu_style][description] = Choose either black or white, vertical or horizontal. This style can be used on menus and uniform lists.
skinr[menu_style][features][] = block
skinr[menu_style][features][] = panels_pane
skinr[menu_style][scripts][] = dropdown.js
skinr[menu_style][stylesheets][all][] = dropdown.css
skinr[menu_style][options][1][label] = Horizontal White
skinr[menu_style][options][1][class] = menu-style menu-horizontal menu-white
skinr[menu_style][options][2][label] = Horizontal Black
skinr[menu_style][options][2][class] = menu-style menu-horizontal menu-black
skinr[menu_style][options][3][label] = Vertical White
skinr[menu_style][options][3][class] = menu-style menu-vertical menu-white
skinr[menu_style][options][4][label] = Vertical Black
skinr[menu_style][options][4][class] = menu-style menu-vertical menu-black

## 7. Advanced Options

### Applying additional CSS classes

In addition to defining CSS classes in the .info file, you can also add classes directly into the UI. Under "Advanced Options" there is a text field where you can add a class or classes manually. Do not include "." in here. Syntax for adding classes here is:

1. One Class:
    my-class

2. Multiple Classes (separated by spaces):
    my-first-class my-second-class

### Using custom template files (WARNING: EXPERIMENTAL)

WARNING: This feature is still in experimental stages, therefore it should NOT be used on live sites. There are still issues with it that need to be worked out, and in general it will NOT be supported at the moment.  Please do not report and issues on this unless they include patches or serious ideas about a way forward for this.

At the moment, templates can be associated with a Skin, or just on their own:

skinr[rounded_corners][templates][] = rounded-corners-block
skinr[rounded_corners][templates][] = rounded-corners-node

However, associating a template with a Skin doesn't automatically mean that file will be used.  It needs to be selected manually in the UI.

Also, since there are different variables and template file structures available per hook, it's necessary to provide template files for each type; hence, the -block, -node suffix.  While this can still be incredibly useful as is, it's obviously not ideal. An implementation such as rounded corners would probably be better served as some type of wrapper template (like comment-wrapper does today). Ideas and patches to solve this issue are more than welcome.

Some creative uses for template files could include:

skinr[extras][templates][] = minimal-block
skinr[extras][templates][] = blank-block

By not specifying the regular widget options, like [title] and [description] for Skins, the widget is conveniently hidden in the UI, but the template still exists under advanced options. So, if you have a case where you don't need the extra markup like the block title and <div class="content">, you can switch the template file to use minimal-block.tpl.php and with the below in your template markup you'll only get the $skinr class and the block content.

In the above example, minimal-block.tpl.php could contain the following markup:

<div class="<?php print $skinr; ?>">
  <?php print $block->content; ?>
</div>

You could also specify a blank-block.tpl.php, which could contain only the content:

<?php print $block->content; ?>

Both of these examples are useful for cases where you might otherwise be creating multiple template files for custom blocks just to remove markup, which can get unruly. In this case you can just assign the appropriate template file and be done with it, while keeping the number of template files in your theme manageable.

## 8. Inheriting Skinr styles from a base theme

If you are creating a subtheme and the base theme you are using contains it's own skinr styles, Skinr allows you to choose whether or not you want your subtheme to inherit those skins or not.  By default Skinr will NOT inherit skins.  If you want to be able to use a base theme's skins, you will need to add this line to your .info file.

skinr[options][inherit_skins] = true

## 9. Making Skinr work with your theme.

Skinr creates a variable containing your skins class that you need to print inside your template files. Thankfully this will not be necessary in Drupal 7, but for now you'll need to add the $skinr variable and all the template files you plan to use yourself.

### Adding the $skinr variable

The $skinr variable should be added to the first, or outer-most <div> in your theme's template files. The variable is the same no matter which type you are dealing with, but you may need to add template files to your theme. The variable is added by doing:

<?php print $skinr; ?>

NOTE: Depending on how your template files are structured, you may need to add a blank space before the variable so it doesn't conflict with other classes you might have:

<?php print ' '. $skinr; ?>

If you are using a theme that takes care of classes during preprocess, like Zen or Studio, you can add $vars['skinr'] there.

### Template files needed in your theme

Here is a list of template files that are typically used and supported by Skinr. See the link for each if you need to create these from scratch in your theme. These links point to Drupal's default code for the template files. You'll be able to copy/paste that code or tpl file to your theme. Then you can add the $skinr variable and modify the code to suit your needs.

* core: [modules/system/block.tpl.php](http://api.drupal.org/api/drupal/modules--system--block.tpl.php/6/source)
* core: [modules/node/node.tpl.php](http://api.drupal.org/api/drupal/modules--node--node.tpl.php/6/source)
* core: [modules/comment/comment-wrapper.tpl.php](http://api.drupal.org/api/drupal/modules--comment--comment-wrapper.tpl.php/6/source)
* contrib: [sites/all/modules/views/theme/views-view.tpl.php](http://views.doc.logrus.com/views-view_8tpl_8php-source.html)

*Note: No template file is required for Panels*
*Note: If you have other template files, i.e. node-blog.tpl.php make sure you print the $skinr variable in them, otherwise it wont work.*

## 10. Some very simple examples

skinr[rounded][title] = Rounded corners
skinr[rounded][description] = Add a 1em border radius for Mozilla and Webkit users.
skinr[rounded][options][1][label] = Rounded Corners
skinr[rounded][options][1][class] = rounded

In your CSS file, you might have:

.rounded {
  -webkit-border-radius: 1em;
  -moz-border-radius: 1em;
}

skinr[big_bold][title] = Big Bold Headers
skinr[big_bold][description] = Title is 16px and font is bold.
skinr[big_bold][option][1][label] = Big Bold Headers
skinr[big_bold][option][1][class] = big-bold

In your CSS file, you might have:

.big-bold {
  font-weight: bold;
  font-size: 16px;
}

For more, please visit: http://jacine.net/demo/skinr-ui.html

# Known Issues:

1. General:
    * Template files as described in #7 don't work properly when placed in theme subfolders.
2. Views:
    * Template files for view display types will not work, i.e. views-view--page.tpl.php
3. Panels: 
    * When applying a Skin to a panel, you must save the panel first, then apply the skinr style and save again.
    * Since there are no template files provided with Panels, the template file options as described in #7 do not work.