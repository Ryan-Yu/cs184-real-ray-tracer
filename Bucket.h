#ifndef BUCKET_H_
#define BUCKET_H_

class Bucket {
	public:
		std::vector<Sample> samples;
		Point anchorPoint;

	Bucket() {

	}

	Bucket(std::vector<Sample> inputSamples, Point anchorPoint) {
		this->samples = inputSamples;
		this->anchorPoint = anchorPoint;
	}

	void addSample(Sample sample) {
		samples.push_back(sample);
	}

};

#endif /* BUCKET_H_ */
