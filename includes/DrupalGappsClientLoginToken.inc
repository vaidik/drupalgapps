<?php

class DrupalGappsClientLoginToken {
	private $email;
	private $service;
	private $source;
	private $token;

	function __construct($email, $source, $service) {
		$this->email = $email;
		$this->source = $source;
		$this->service = $service;
		$this->setToken(NULL);
	}

	function fetchToken($password) {
		$http_client = new HttpClient();

		$request_values = array(
			'method' => 'POST',
			'parameters' => array(
				'Email' => $this->email,
				'Passwd' => $password,
				'accountType' => 'GOOGLE',
				'source' => $this->source,
				'service' => $this->service,
			),
		);

		$url = 'https://www.google.com/accounts/ClientLogin';
		$request = new HttpClientRequest($url, $request_values);

		$response = $http_client->execute($request);
		$lastResponse = $http_client->lastResponse;
		$responseCode = $lastResponse->responseCode;

		if ($responseCode == 200) {
			$response_lines = explode("\n", $response);
			foreach ($response_lines as $line) {
				if (preg_match('/^Auth=(.+)$/', $line, $matches)) {
					$auth_token = $matches[1];
					$this->setToken($auth_token);
					break;
				}
			}
		}
	}

	public function getToken() {
		return $this->token;
	}

	public function setToken($token) {
		$this->token = $token;
	}
}
