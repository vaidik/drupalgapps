<?php

abstract class DrupalGappsRequest {
	private $token;

	function __construct($token) {
		$this->setToken($token);
	}

	function setToken($token) {
		$this->token = $token;
	}

	function getToken() {
		return $this->token;
	}

	abstract function prepareClient();

	public function execute($url, array $options = array('method' => 'GET', 'parameters' => array(), 'headers' => array()), $data = NULL) {
		$this->prepareClient();
		$http_client = $this->http_client;

		$request_values = array(
      'method' => $options['method'],
    );

		foreach ($options['parameters'] as $params => $value) {
			$request_values['parameters'][$params] = $value;
		}

		$request = new HttpClientRequest($url, $request_values);

		foreach ($options['headers'] as $header => $value) {
			$request->addHeader($header, $value);
		}

		$response = $http_client->execute($request);

		return $response;
	}
}

class DrupalGappsClientLoginRequest extends DrupalGappsRequest {
	function prepareClient() {
		$this->http_client = new HttpClient();
	}

	function execute($url, array $options = array('method' => 'GET', 'parameters' => array(), 'headers' => array()), $data = NULL) {

		$options['headers'] += array(
			'Authorization' => 'GoogleLogin auth=' . parent::getToken()->getToken(),
			'GData-Version' => '3.0',
		);

		return parent::execute($url, $options, $data);
	}
}
