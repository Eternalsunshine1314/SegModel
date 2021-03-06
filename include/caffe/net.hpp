// Copyright 2013 Yangqing Jia

#ifndef CAFFE_NET_HPP_
#define CAFFE_NET_HPP_

#include <map>
#include <string>
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/common.hpp"
#include "caffe/proto/caffe.pb.h"
#include "boost/scoped_ptr.hpp"
#include <boost/thread.hpp>

using std::map;
using std::vector;
using std::string;

namespace caffe {

template <typename Dtype>
class Net
{
 public:
	Net(const NetParameter& param, const vector<shared_ptr<Blob<Dtype> > > net_input_blobs, const vector<string> net_input_blob_names);
  ~Net();
  Dtype Forward();
  void Backward();
  void SecForward();

	void Update(int iter, int max_iter, bool display);

  void CopyTrainedLayersFrom(const NetParameter& param);
	void ClearParamDiffs();
  void ToProto(NetParameter* param, bool write_diff = false);
	

  inline const string& name() { return name_; }
  inline const vector<shared_ptr<Layer<Dtype> > >& layers() { return layers_; }
  inline const vector<string>& layer_names() { return layer_names_; }
  
  inline const vector<shared_ptr<Blob<Dtype> > >& blobs() { return blobs_; }
  inline const vector<string>& blob_names() { return blob_names_; }
	inline const vector<Dtype>& blob_loss_weights() const { return blob_loss_weights_; }
	inline vector<int>& output_blob_indices() { return output_blob_indices_; }  
	inline const vector<int>& input_blob_indices() const { return input_blob_indices_; }
	
	void BcastData();
	void ReduceDiff();
	void ScaleDiff(const Dtype scalar);
	 
  inline const vector<Blob<Dtype>*>& output_blobs() const { return output_blobs_; }
  inline const vector<Blob<Dtype>*>& input_blobs() const { return input_blobs_; }

  inline vector<vector<Blob<Dtype>*> >& bottom_vecs() { return bottom_vecs_; }
  inline vector<vector<Blob<Dtype>*> >& top_vecs() { return top_vecs_; }
	
	

	
	Dtype GetLearningRate(int iter, int max_iter);
	void Snapshot();
	
	void StateToProto(NetState * state);
	void RestoreState(const NetState  state);
	void set_NetOptimizer(const NetOptimizer net_opt) { optimizer_ = net_opt;}
 protected:
 	vector<bool> layer_need_backward_;
 	
  vector<shared_ptr<Layer<Dtype> > > layers_;
	vector<string> layer_names_;
	
  vector<shared_ptr<Blob<Dtype> > > blobs_;
  vector<string> blob_names_;
  vector<Dtype> blob_loss_weights_;
  vector<int> input_blob_indices_;
  vector<int> output_blob_indices_;
  
  
  vector<vector<Blob<Dtype>*> > bottom_vecs_;
  vector<vector<int> > bottom_id_vecs_;
  vector<vector<Blob<Dtype>*> > top_vecs_;
  vector<vector<int> > top_id_vecs_;
  
 	vector<shared_ptr<Blob<Dtype> > > tensor_flows_;
  vector<shared_ptr<Blob<Dtype> > > tensor_flows_temp_;
  
	vector<Blob<Dtype>*> output_blobs_;
	vector<Blob<Dtype>*> input_blobs_;
	
  string name_;

	
	NetParameter param_;
	NetOptimizer optimizer_;
	
	vector<bool> flow_flag;
	int adam_iter_;
	Dtype  momentum_power_;
	Dtype momentum2_power_;
	
	shared_ptr<boost::thread> thread_;
	
  DISABLE_COPY_AND_ASSIGN(Net);
};


}  // namespace caffe

#endif  // CAFFE_NET_HPP_
