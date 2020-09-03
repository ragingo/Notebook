(function () {
  window.onload = function () {
    const ua = window.navigator.userAgent.toLowerCase();
    if(ua.indexOf('msie') != -1 || ua.indexOf('trident') != -1) {
      const msg = document.querySelector('.LEDMessageBoard-message');
      msg.setAttribute('data-is-ie');
    }
  };
})();