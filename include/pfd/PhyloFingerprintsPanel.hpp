#pragma once
#ifndef PFD_PHYLOFINGERPRINTSPANEL_HPP_INCLUDE
#define PFD_PHYLOFINGERPRINTSPANEL_HPP_INCLUDE

#include "emp/base/array.hpp"
#include "emp/math/math.hpp"
#include "emp/tools/hash_namify.hpp"
#include "emp/web/Color.hpp"
#include "emp/web/Div.hpp"

#include "../../third-party/conduit/include/uitsl/nonce/ScopeGuard.hpp"
#include "../../third-party/dishtiny/include/dish2/genome/PhyloFingerprints.hpp"

namespace pfd {

namespace internal {
/**

 * Shared pointer held by instances of PhyloFingerprintsPanel class representing
 * the same conceptual PhyloFingerprintsPanel DOM object.
 * Contains state that should persist while PhyloFingerprintsPanel DOM object
 * persists.
 */
struct PhyloFingerprintsPanelInfo : public emp::web::internal::DivInfo {

  /**
   * Construct a shared pointer to manage Card state.
   * @param in_id HTML ID of Card div
   */
  PhyloFingerprintsPanelInfo(const std::string & in_id="")
  : DivInfo(in_id)
  { ; }

  emp::vector<dish2::PhyloFingerprints> fingerprints{1};

};

} // namespace internal

/// Use Card class to create Bootstrap style cards.
class PhyloFingerprintsPanel : public emp::web::Div {

  /**
  * Get shared info pointer, cast to Card-specific type.
  * @return cast pointer
  */
  internal::PhyloFingerprintsPanelInfo * Info() {
   return dynamic_cast<internal::PhyloFingerprintsPanelInfo *>(info);
  }

  emp::web::Table table{2, 2};

  size_t get_fingerprint_col(const size_t fingerprint_idx) const {
    return fingerprint_idx + 1;
  }

  size_t get_generation_row(const size_t generation) const {
    return generation + 1;
  }

  emp::web::Div make_fingerprint_controls(const size_t fingerprint_idx) {
    emp::web::Div res;
    res << emp::web::Button(
      [panel=*this, fingerprint_idx]() mutable {
        panel.StepFingerprint(fingerprint_idx);
      },
      "Step"
    );
    res << emp::web::Button(
      [panel=*this, fingerprint_idx]() mutable {
        panel.DuplicateFingerprint(fingerprint_idx);
      },
      "Duplicate"
    );
    return res;
  }

public:

  PhyloFingerprintsPanel(
    const std::string& in_id=""
  ) : PhyloFingerprintsPanel(new internal::PhyloFingerprintsPanelInfo(in_id))
  { ; }

protected:

  PhyloFingerprintsPanel(
    internal::PhyloFingerprintsPanelInfo * in_info
  ) : Div(in_info) {

    table.SetCSS( "width", "100%" );
    table.SetCSS(
      "border", "2px solid white",
      "border-collapse", "separate"
    );
    this->SetCSS( "height", "100%" );

    table.GetCell(
      0,
      get_fingerprint_col(0)
    ) << make_fingerprint_controls(0);

    table.GetCell(0, 0) << "gen";

    // add generation label
    table.GetCell(
      get_generation_row(0),
      0
    ) << 0;


    static_cast<emp::web::Div>(*this) << table;

    RedrawFingerprint(0);

  }

  void RedrawFingerprint( const size_t fingerprint_idx ) {

    // disable rendering to reduce lagginess
    const uitsl::ScopeGuard freeze_guard{
      [&](){ table.Freeze(); },
      [&](){ table.Activate(); }
    };

    auto& fingerprint = Info()->fingerprints.at( fingerprint_idx );

    for (size_t gen{}; gen < fingerprint.GetGenerationCount(); ++gen) {

      const auto found_generation = std::find(
        std::begin(fingerprint.GetGenerations()),
        std::end(fingerprint.GetGenerations()),
        gen
      );
      const bool fingerprint_retained = (
        found_generation != std::end(fingerprint.GetGenerations())
      );

      table.ClearCell(
        get_generation_row(gen),
        get_fingerprint_col( fingerprint_idx )
      );

      if (fingerprint_retained) {
        const size_t hash = fingerprint.GetFingerprint( std::distance(
          std::begin(fingerprint.GetGenerations()),
          found_generation
        ) );
        table.GetCell(
          get_generation_row(gen),
          get_fingerprint_col( fingerprint_idx )
        ).SetCSS(
          "background-color",
          emp::ColorHSV(
            emp::Mod(hash, 360.0),
            emp::Mod(hash, 0.2) + 0.2,
            1.0
          )
        ).SetCSS(
          "font-weight",
          "bold",
          "color",
          "black"
        ) << emp::hash_namify( hash );
      } else table.GetCell(
        get_generation_row(gen),
        get_fingerprint_col( fingerprint_idx )
      ).SetCSS(
        "background-color",
        "gainsboro"
      );

    }

    for (
      size_t row = get_generation_row( fingerprint.GetGenerationCount() );
      row < table.GetNumRows();
      ++row
    ) table.ClearCell(
      row,
      get_fingerprint_col( fingerprint_idx )
    );

  }

public:

  void DuplicateFingerprint(const size_t fingerprint_idx) {
    Info()->fingerprints.insert(
      std::begin(Info()->fingerprints) + fingerprint_idx,
      // copy construct to prevent dangling reference after vector resize
      dish2::PhyloFingerprints(Info()->fingerprints[ fingerprint_idx ])
    );

    // add additional column
    table.Cols( table.GetNumCols() + 1 );
    table.GetCol(
      table.GetNumCols() - 1
    ).SetCSS(
      "border",
      "2px solid white"
    );

    table.GetCell(
      0,
      get_fingerprint_col( Info()->fingerprints.size()- 1 )
    ) << make_fingerprint_controls( Info()->fingerprints.size()- 1 );

    for (size_t i = fingerprint_idx + 1; i < Info()->fingerprints.size(); ++i) {
      RedrawFingerprint( i );
    }

  }

  void StepFingerprint(const size_t fingerprint_idx) {

    auto& fingerprint = Info()->fingerprints.at( fingerprint_idx );

    fingerprint.ElapseGeneration();

    const size_t cur_generation = fingerprint.GetGenerationCount() - 1;

    if (
      get_generation_row(cur_generation) == table.GetNumRows()
    ) {
      // append row
      table.Rows(
        get_generation_row(cur_generation) + 1
      );
      // append row
      table.GetRow(
        get_generation_row(cur_generation)
      ).SetCSS(
        "border",
        "2px solid white"
      );
      // add generation label
      table.GetCell(
        get_generation_row(cur_generation),
        0
      ) << cur_generation;
    }

    RedrawFingerprint( fingerprint_idx );

  }

};

} // namespace pfd

#endif // #ifndef PFD_PHYLOFINGERPRINTSPANEL_HPP_INCLUDE
