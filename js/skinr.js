// $Id$

(function ($) {

// Make sure our objects are defined.
Drupal.CTools = Drupal.CTools || {};
Drupal.Skinr = Drupal.Skinr || {};

Drupal.behaviors.Skinr = {
  attach: function(context, settings) {
    for (var i in settings.skinr['areas']) {
      var $id = settings.skinr['areas'][i]['id'];
      var $module = settings.skinr['areas'][i]['module'];
      var $sid = settings.skinr['areas'][i]['sid'];

      var $region = $('.skinr-id-' + $id).once('skinr-region', function() {});

      if ($region.length > 0) {
        if ($.isArray($sid)) {
          var $links = '';
          for (var $j in $sid) {
            var $classes = '';
            if ($j == 0) {
              $classes += ' first';
            }
            if ($j == $sid.length - 1) {
              $classes += ' last';
            }
            $links += '<li class="skinr-link-' + $j + $classes + '"><a href="/admin/settings/skinr/nojs/' + $module + '/' + $sid[$j] + '/' + $id +'" class="skinr-link ctools-use-dialog">' + Drupal.t('Edit skin') + ' ' + (parseInt($j) + 1) + '</a></li>';
          }
        }
        else {
          $links = '<li class="skinr-link-0 first last"><a href="/admin/settings/skinr/nojs/' + $module + '/' + $sid + '/' + $id +'" class="skinr-link ctools-use-dialog">' + Drupal.t('Edit skin') + '</a></li>';
        }

        var $wrapper_classes = '';
        if ($module == 'page') {
          $wrapper_classes += ' skinr-links-wrapper-page';
        }

        $region.prepend('<div class="skinr-links-wrapper' + $wrapper_classes + '"><ul class="skinr-links">' + $links + '</ul></div>');
        $region.get(0).skinr = { 'module': $module, 'sid': $sid, 'id': $id };

        Drupal.behaviors.Dialog($region);
      };
    }

    $('div.skinr-links-wrapper', context).once('skinr-links-wrapper', function () {
      var $wrapper = $(this);
      var $region = $wrapper.closest('.skinr-region');
      var $links = $wrapper.find('ul.skinr-links');
      var $trigger = $('<a class="skinr-links-trigger" href="#" />').text(Drupal.t('Configure')).click(
        function () {
          $wrapper.find('ul.skinr-links').stop(true, true).slideToggle(100);
          $wrapper.toggleClass('skinr-links-active');
          return false;
        }
      );

      // Attach hover behavior to trigger and ul.skinr-links.
      $trigger.add($links).hover(
        function () { $region.addClass('skinr-region-active'); },
        function () { $region.removeClass('skinr-region-active'); }
      );
      // Hide the contextual links when user rolls out of the .skinr-links-region.
      $region.bind('mouseleave', Drupal.Skinr.hideLinks).click(Drupal.Skinr.hideLinks);
      // Prepend the trigger.
      $links.end().prepend($trigger);
    });
  }
};

/**
 * Disables outline for the region contextual links are associated with.
 */
Drupal.Skinr.hideLinks = function () {
  $(this).closest('.skinr-region')
    .find('.skinr-links-active').removeClass('skinr-links-active')
    .find('ul.skinr-links').hide();
};

/**
 * AJAX responder command to dismiss the modal.
 */
Drupal.CTools.AJAX.commands.skinr_afterupdate = function(command) {
  if (command.id && (command.classes.remove || command.classes.add)) {
    if (command.css) {
      for (var j in command.css) {
        $(document.createElement('link')).attr({href: command.css[j].path, media: command.css[j].media, rel: 'stylesheet', type: 'text/css'}).appendTo('head');
      }
    }
    if (command.js) {
      for (var j in command.js) {
        $.getScript(command.js[j].path);
      }
    }
    $('.skinr-id-'+command.id).removeClass(command.classes.remove).addClass(command.classes.add);
  }
}

})(jQuery);
