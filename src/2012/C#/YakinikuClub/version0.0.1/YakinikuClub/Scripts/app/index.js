jQuery(function ($) {

	$('#top_map').height(
			$('#top_map').height() - $('#info').height() - 30
		);

	var map = new google.maps.Map($('#top_map')[0], {
		mapTypeId: google.maps.MapTypeId.ROADMAP,
		scaleControl: true,
		zoom: 17
	});
	var marker = new google.maps.Marker({ map: map });
	var geocoder = new google.maps.Geocoder();

	if (!navigator.geolocation) {
		$("top_map").text("位置情報を取得できませんでした。");
		return;
	}
	navigator.geolocation.getCurrentPosition(function (pos) {
		var latlng = new google.maps.LatLng(pos.coords.latitude, pos.coords.longitude);
		var req = { 'latLng': latlng };
		geocoder.geocode(req, function (results, status) {
			if (status != google.maps.GeocoderStatus.OK) {
				return;
			}
			map.setCenter(latlng);
			marker.setPosition(latlng);
			$('#latlng').text(latlng.toString());
			$('#address').text(results[0].formatted_address);
		});
	});
});
